import argparse
import gym
import os

import random
import numpy as np

import ray
from ray import air, tune
from ray.rllib.algorithms.registry import get_algorithm_class
from ray.rllib.env.policy_server_input import PolicyServerInput
from ray.rllib.examples.custom_metrics_and_callbacks import MyCallbacks
from ray.tune.logger import pretty_print
from ray.tune.schedulers import ASHAScheduler

import torch

print("CUDA VER: "+str(torch.version.cuda))
print("CUDA AVAIL: "+str(torch.cuda.is_available()))
print("CUDA DEV COUNT: "+str(torch.cuda.device_count()))

# print("RAY GPU IDS: "+str(ray.get_gpu_ids()))

SERVER_ADDRESS = "localhost"
# In this example, the user can run the policy server with
# n workers, opening up listen ports 9900 - 990n (n = num_workers - 1)
# to each of which different clients may connect.
SERVER_BASE_PORT = 9900  # + worker-idx - 1

CHECKPOINT_FILE = "last_checkpoint_{}.out"


def deep_merge(a, b, path=None):
    "merges b into a"
    if path is None: path = []
    for key in b:
        if key in a:
            if isinstance(a[key], dict) and isinstance(b[key], dict):
                deep_merge(a[key], b[key], path + [str(key)])
            else:
                a[key] = b[key]
        else:
            a[key] = b[key]
    return a


def get_cli_args():
    """Create CLI parser and return parsed arguments"""
    parser = argparse.ArgumentParser()

    # Example-specific args.
    parser.add_argument(
        "--port",
        type=int,
        default=SERVER_BASE_PORT,
        help="The base-port to use (on localhost). " f"Default is {SERVER_BASE_PORT}.",
    )
    parser.add_argument(
        "--callbacks-verbose",
        action="store_true",
        help="Activates info-messages for different events on "
        "server/client (episode steps, postprocessing, etc..).",
    )
    parser.add_argument(
        "--num-workers",
        type=int,
        default=2,
        help="The number of workers to use. Each worker will create "
        "its own listening socket for incoming experiences.",
    )
    parser.add_argument(
        "--no-restore",
        action="store_true",
        help="Do not restore from a previously saved checkpoint (location of "
        "which is saved in `last_checkpoint_[algo-name].out`).",
    )

    # General args.
    parser.add_argument(
        "--run",
        default="PPO",
        choices=["APEX", "DQN", "IMPALA", "PPO", "R2D2"],
        help="The RLlib-registered algorithm to use.",
    )
    parser.add_argument("--num-cpus", type=int, default=2)
    parser.add_argument(
        "--framework",
        choices=["tf", "tf2", "tfe", "torch"],
        default="torch",
        help="The DL framework specifier.",
    )
    parser.add_argument(
        "--use-lstm",
        action="store_true",
        help="Whether to auto-wrap the model with an LSTM. Only valid option for "
        "--run=[IMPALA|PPO|R2D2]",
    )
    parser.add_argument(
        "--stop-iters", type=int, default=200, help="Number of iterations to train."
    )
    parser.add_argument(
        "--stop-timesteps",
        type=int,
        default=1000000000,
        help="Number of timesteps to train.",
    )
    parser.add_argument(
        "--stop-reward",
        type=float,
        default=999999.0,
        help="Reward at which we stop training.",
    )
    parser.add_argument(
        "--as-test",
        action="store_true",
        help="Whether this script should be run as a test: --stop-reward must "
        "be achieved within --stop-timesteps AND --stop-iters.",
    )
    parser.add_argument(
        "--no-tune",
        action="store_true",
        help="Run without Tune using a manual train loop instead. Here,"
        "there is no TensorBoard support.",
    )
    parser.add_argument(
        "--local-mode",
        action="store_true",
        help="Init Ray in local mode for easier debugging.",
    )

    # parser.add_argument( does not work ATM
    #     "--num-samples",
    #     type=int,
    #     default=20,
    #     help="Number of samples to train",
    # )

    parser.add_argument(  # Does not work ATM
        "--use-scheduler",
        action="store_true",
        help="Use a ASHA Scheduler",
    )

    parser.add_argument(  # Does not work ATM
        "--use-tune-config",
        action="store_true",
        help="Use a ASHA Scheduler",
    )

    args = parser.parse_args()
    print(f"Running with following CLI args: {args}")
    return args


if __name__ == "__main__":
    args = get_cli_args()
    ray.init()

    # `InputReader` generator (returns None if no input reader is needed on
    # the respective worker).
    def _input(ioctx):
        # We are remote worker or we are local worker with num_workers=0:
        # Create a PolicyServerInput.
        if ioctx.worker_index > 0 or ioctx.worker.num_workers == 0:
            return PolicyServerInput(
                ioctx,
                SERVER_ADDRESS,
                args.port + ioctx.worker_index - (1 if ioctx.worker_index > 0 else 0),
            )
        # No InputReader (PolicyServerInput) needed.
        else:
            return None

    # Algorithm config. Note that this config is sent to the client only in case
    # the client needs to create its own policy copy for local inference.
    config = {
        # Indicate that the Algorithm we setup here doesn't need an actual env.
        # Allow spaces to be determined by user (see below).
        "env": None,
        # TODO: (sven) make these settings unnecessary and get the information
        #  about the env spaces from the client.
        "observation_space": gym.spaces.Box(float("-inf"), float("inf"), (955,)), # TODO find dynamic way to set this value
        "action_space": gym.spaces.Discrete(18), # TODO find dynamic way to set this value
        # Use the `PolicyServerInput` to generate experiences.
        "input": _input,
        # Use n worker processes to listen on different ports.
        "num_workers": args.num_workers,
        "num_gpus": 1,
        "off_policy_estimation_methods": {},
        # Create a "chatty" client/server or not.
        "callbacks": MyCallbacks if args.callbacks_verbose else None,
        # DL framework to use.
        "framework": args.framework,
        # Set to INFO so we'll see the server's actual address:port.
        "log_level": "INFO",
        "model": {},
    }

    # DQN.
    if args.run == "DQN" or args.run == "APEX" or args.run == "R2D2":
        # Example of using DQN (supports off-policy actions).
        config.update(
            {
                "num_steps_sampled_before_learning_starts": 100,
                "min_sample_timesteps_per_iteration": 200,
                "n_step": 3,
                "rollout_fragment_length": 4,
                "train_batch_size": 8,
            }
        )
        config["model"] = {
            "fcnet_hiddens": [64],
            "fcnet_activation": "linear",
        }
        if args.run == "R2D2":
            config["model"]["use_lstm"] = args.use_lstm

    elif args.run == "IMPALA":
        config.update(
            {
                "num_gpus": 0,
                "model": {"use_lstm": args.use_lstm},
            }
        )

    # PPO.
    else:
        # Example of using PPO (does NOT support off-policy actions).
        config.update(
            {
                #"lr": 0.0001,
                "lr_schedule": [
                    [0, 5e-5],  # 0? lr_start
                    [args.stop_timesteps, 0.0000000001],  # lr_time, lr_end
                ],
                "gamma": 0.999,
                "lambda": 0.99,
                "kl_target": 0.01,
                "kl_coeff": 0.2,
                "clip_param": 0.3,
                "vf_loss_coeff": 1.0,
                "vf_clip_param": 5.0,
                "entropy_coeff": 0.005,
                "num_sgd_iter": 30,
                "sgd_minibatch_size": 128,
                "rollout_fragment_length": 10000,
                "train_batch_size": 10000,
                "model": {
                    "fcnet_hiddens": [1024, 1024],
                    "fcnet_activation": "tanh",
                    # "conv_filters": null,
                    # "conv_activation": "relu",
                },
            }
        )

        if args.use_tune_config:
            tune_config = {
                'lr': tune.uniform(0.00005, 0.001),
                'gamma': tune.uniform(0.99, 0.999),
                'lambda': tune.sample_from(
                    lambda _: np.clip(np.random.normal(0.95, 0.02), 0.9, 1.0)),
                'kl_target': tune.uniform(0.001, 0.2),
                'kl_coeff': tune.sample_from(
                    lambda _: np.clip(np.random.normal(1.00, 0.02), 0.5, 1.0)),
                'clip_param': tune.sample_from(
                    lambda _: np.clip(np.random.normal(0.2, 0.01), 0.1, 0.5)),
                'vf_loss_coeff': tune.uniform(0.1, 1.0),
                'vf_clip_param': tune.uniform(0.1, 10.0),
                'entropy_coeff': tune.uniform(0.0, 0.05),
                'num_sgd_iter': tune.randint(5, 20),
                'model': {
                    'vf_share_layers': tune.choice(["true", "false"]),
                    'use_lstm': tune.choice(["true", "false"]),
                    'max_seq_len': tune.qrandint(10, 20),
                    'fcnet_hiddens': tune.sample_from(
                        lambda _: random.sample([
                            [256, 256],
                            [128, 256],
                            [256, 128],
                            [128, 128],
                        ], 1)[0]),
                    'fcnet_activation': tune.choice(["tanh", "relu"]),
                    'lstm_cell_size': tune.choice([128, 256]),
                    'lstm_use_prev_action': tune.choice(["true", "false"]),
                    'lstm_use_prev_reward': tune.choice(["true", "false"]),
                }
            }

            config = deep_merge(config, tune_config)

    if args.use_scheduler:
        scheduler = ASHAScheduler(
            time_attr='timesteps_total',
            metric='episode_reward_mean',
            mode='max',
            max_t=args.stop_timesteps,
            grace_period=int(args.stop_timesteps * 0.10),
            reduction_factor=2,
            brackets=4)
        stop = None
    else:
        scheduler = None

    print("model: "+str(config["model"]))

    checkpoint_path = CHECKPOINT_FILE.format(args.run)
    # Attempt to restore from checkpoint, if possible.
    if not args.no_restore and os.path.exists(checkpoint_path):
        checkpoint_path = open(checkpoint_path).read()
    else:
        checkpoint_path = None

    # Manual training loop (no Ray tune).
    if args.no_tune:
        algo_cls = get_algorithm_class(args.run)
        algo = algo_cls(config=config)

        if checkpoint_path:
            print("Restoring from checkpoint path", checkpoint_path)
            algo.restore(checkpoint_path)

        # Serving and training loop.
        ts = 0
        for _ in range(args.stop_iters):
            results = algo.train()
            print(pretty_print(results))
            checkpoint = algo.save()
            print("Last checkpoint", checkpoint)
            with open(checkpoint_path, "w") as f:
                f.write(checkpoint)
            if (
                results["episode_reward_mean"] >= args.stop_reward
                or ts >= args.stop_timesteps
            ):
                break
            ts += results["timesteps_total"]

    # Run with Tune for auto env and algo creation and TensorBoard.
    else:
        print("Ignoring restore even if previous checkpoint is provided...")
        stop = {
            # "training_iteration": args.stop_iters,
            "timesteps_total": args.stop_timesteps,
            "episode_reward_mean": args.stop_reward,
        }

        tune.Tuner(
            args.run,
            tune_config=tune.TuneConfig(
                # num_samples=args.num_samples,
                scheduler=scheduler,
            ),
            param_space=config,
            run_config=air.RunConfig(
                stop=stop,
                verbose=2,
                local_dir="results",
            )
        ).fit()
