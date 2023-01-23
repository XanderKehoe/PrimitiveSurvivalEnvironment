# PrimitiveSurvivalEnvironment

A reinforcement learning environment where the goal is to have an agent learn to survive in a harsh stone-era inspired environment. Starting with no food, equipment, or even knowledge of how the world works; the agent must learn to find food, craft equipment, and fend off hostile animals in order to survive.

The environment was created in C++ through the Simple DirectMedia Layer 2.0 (SDL2) library. Shared interprocess memory is used to communicate between the environment and with a Python client script which preproccess the data before being sent to a Python server script which handles the training of the agent. These python scripts utilize Ray's RLlib to support easy use of a wide range of RL algorithms with exceptional performance.

The AI of the animals in the environment utilize a Finite State Machine to control their behavior. Hostile animals utilize A* pathfinding to navigate towards the agent, while passive animals use it to navigate away from the agent.
