import argparse
import gym
import os

import random
import numpy as np

import ray
# from ray import air, tune
# from ray.rllib.algorithms.registry import get_algorithm_class
# from ray.rllib.env.policy_server_input import PolicyServerInput
# from ray.rllib.examples.custom_metrics_and_callbacks import MyCallbacks
# from ray.tune.logger import pretty_print
# from ray.tune.schedulers import ASHAScheduler

import torch

print("CUDA VER: "+str(torch.version.cuda))
print("CUDA AVAIL: "+str(torch.cuda.is_available()))
print("CUDA DEV COUNT: "+str(torch.cuda.device_count()))

# print("RAY GPU IDS: "+str(ray.get_gpu_ids()))