import struct
from multiprocessing import shared_memory
import numpy as np
import array

# TODO test all of these functions

def ReadFloatFromBytes(bytesArg: list):
    return memoryview(bytes(bytesArg)).cast('f')[0]


def ReadIntFromBytes(bytesArg: list):
    bytesArg = reversed(bytesArg)
    return int.from_bytes(bytesArg, byteorder='big', signed=False)


def ReadStateAndRewardFromBuffer():
    global shm
    reward = ReadIntFromBytes([shm.buf[4], shm.buf[5], shm.buf[6], shm.buf[7]])
    numberOfStates = ReadIntFromBytes([shm.buf[8], shm.buf[9], shm.buf[10], shm.buf[11]])
    states = []
    for i in range(12, numberOfStates, 4):
        states.append(ReadFloatFromBytes([shm.buf[8], shm.buf[9], shm.buf[10], shm.buf[11]]))

    return states, reward


def ClearBuffer():
    for i in range(len(shm.buf)):
        shm.buf[i] = 0


if __name__ == "__main__":
    shm = shared_memory.SharedMemory(create=False, name="PSEMem")
    states, reward = ReadStateAndRewardFromBuffer()
    # while True:

