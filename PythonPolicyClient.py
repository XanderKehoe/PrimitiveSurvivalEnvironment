import struct
from multiprocessing import shared_memory
import numpy as np
import array
import ctypes

import argparse

from ray.rllib.env.policy_client import PolicyClient

# TODO test all of these functions
global shm

NUMBER_OF_BYTES_IN_INT_AND_FLOAT = 4
NUMBER_OF_STATES_PER_TILE = 3
NUMBER_OF_TILE_TYPES = 10
NUMBER_OF_ENTITY_TYPES = 7
SHARED_MEM_STR = "PSEMem"


def ReadFloatFromBytes(bytesArg: list):
    return memoryview(bytes(bytesArg)).cast('f')[0]


def ReadIntFromBytes(bytesArg: list):
    bytesArg = reversed(bytesArg)
    return int.from_bytes(bytesArg, byteorder='big', signed=False)


def ReadStateRewardDoneFromBuffer():
    # print("buf before read: "+str(bytes(shm.buf)))
    r = ReadFloatFromBytes([shm.buf[4], shm.buf[5], shm.buf[6], shm.buf[7]])
    d = ReadIntFromBytes([shm.buf[8], shm.buf[9], shm.buf[10], shm.buf[11]])
    numberOfIntStates = ReadIntFromBytes([shm.buf[12], shm.buf[13], shm.buf[14], shm.buf[15]])
    numberOfFloatStates = ReadIntFromBytes([shm.buf[16], shm.buf[17], shm.buf[18], shm.buf[19]])
    # print("numberOfIntStates: "+str(numberOfIntStates))
    # print("numberOfFloatStates: " + str(numberOfFloatStates))

    s = []
    index = 20
    i = 0
    for i in range(20,
                   20 + (numberOfIntStates * NUMBER_OF_BYTES_IN_INT_AND_FLOAT),
                   NUMBER_OF_BYTES_IN_INT_AND_FLOAT):
        byteArr = [shm.buf[i], shm.buf[i + 1], shm.buf[i + 2], shm.buf[i + 3]]
        intFromBytes = ReadIntFromBytes(byteArr)
        s.append(intFromBytes)
        index += NUMBER_OF_BYTES_IN_INT_AND_FLOAT
        # if ReadIntFromBytes([shm.buf[i], shm.buf[i+1], shm.buf[i+2], shm.buf[i+3]]) > 1.0:
        # print("i G>(int): "+str(i)+" | "+str(ReadIntFromBytes([shm.buf[i], shm.buf[i+1], shm.buf[i+2], shm.buf[i+3]])))

    s = PreprocessStates(s)
    # for k in range(20, len(s), 1):
    #     if s[k] > 1:
    #         print("Preproccessed state["+str(k)+"] > 1: "+str(s[k]))

    for j in range(i + NUMBER_OF_BYTES_IN_INT_AND_FLOAT,
                   i + NUMBER_OF_BYTES_IN_INT_AND_FLOAT + (numberOfFloatStates * NUMBER_OF_BYTES_IN_INT_AND_FLOAT),
                   NUMBER_OF_BYTES_IN_INT_AND_FLOAT):
        byteArr = [shm.buf[j], shm.buf[j + 1], shm.buf[j + 2], shm.buf[j + 3]]
        floatFromBytes = ReadFloatFromBytes(byteArr)
        s.append(floatFromBytes)
        index += NUMBER_OF_BYTES_IN_INT_AND_FLOAT
        if floatFromBytes > 1.0:
            print("WARNING - float > 1: " + str(i))

    s = [float(x) for x in s]
    # s = np.asarray(s, dtype=np.float32)
    # print("len(s) = "+str(len(s)))

    # ReadBuffer()

    return s, r, d


def GetOneHotEncoding(val: int, size: int):
    ret = []
    for x in range(size):
        if x == val:
            ret.append(1)
        else:
            ret.append(0)

    return ret


def PreprocessStates(s):
    ret = []
    for i in range(0, len(s), NUMBER_OF_STATES_PER_TILE):
        # print("\ti(0): " + str(i) + " | len(s) == " + str(len(s)))
        # entityType
        if s[i] > NUMBER_OF_ENTITY_TYPES:
            print("WARNING - PreprocessStates(): invalid entity type - "+str(s[i])+" | "+str(NUMBER_OF_ENTITY_TYPES))
        entityTypeOneHotList = GetOneHotEncoding(s[i], NUMBER_OF_ENTITY_TYPES)
        for x in range(len(entityTypeOneHotList)):
            ret.append(entityTypeOneHotList[x])

        # availability
        # print("\ti(1): "+str(i+1) + " | len(s) == "+str(len(s)))
        if s[i+1] > 1:
            print("WARNING - PreprocessStates(): availability > 1 ["+str(s[i+1])+"] at index: "+str(i+1))
        ret.append(s[i+1])  # availability

        # tileType
        # print("\ti(2): "+str(i+2) + " | len(s) == "+str(len(s)))
        if s[i+2] > NUMBER_OF_TILE_TYPES:
            print("WARNING - PreprocessStates(): invalid type type - "+str(s[i+2])+" | "+str(NUMBER_OF_TILE_TYPES))
        tileTypeOneHotList = GetOneHotEncoding(s[i+2], NUMBER_OF_TILE_TYPES)
        for x in range(len(tileTypeOneHotList)):
            ret.append(tileTypeOneHotList[x])

    return ret


def ReadBuffer():
    for i in range(0, len(shm.buf), NUMBER_OF_BYTES_IN_INT_AND_FLOAT):
        fourBytes = [shm.buf[i + 3], shm.buf[i + 2], shm.buf[i + 1], shm.buf[i]]
        byteBinaryStr = ""
        for byte in fourBytes:
            byteBinaryStr += (str(bin(byte)) + ":")[2:]

        print("i: " + str(i) + " -> " + byteBinaryStr)


def WaitForSharedMemStart():
    global shm
    while True:
        try:
            shm = shared_memory.SharedMemory(create=False, name=SHARED_MEM_STR)
        except Exception as e:
            # If an exception is raised, the shared memory object does not exist
            pass
        else:
            # If no exception is raised, the shared memory object exists
            print("Shared-Memory found, starting...")
            break


def WaitForAvailability():
    while True:
        availability = ReadIntFromBytes([shm.buf[0], shm.buf[1], shm.buf[2], shm.buf[3]])
        if availability == 1:
            break


def ClearBuffer():
    def memsetObject(bufferObject):
        # Note, dangerous
        data = ctypes.POINTER(ctypes.c_char)()
        size = ctypes.c_int()  # Note, int only valid for python 2.5
        ctypes.pythonapi.PyObject_AsCharBuffer(ctypes.py_object(bufferObject), ctypes.pointer(data),
                                               ctypes.pointer(size))
        ctypes.memset(data, 0, size.value)

    memsetObject(shm.buf)


def IntToBytes(x: int):
    return x.to_bytes(4, byteorder='big', signed=False)


def InsertByteArrIntoBuffer(i: int, bytearr: bytes):
    shm.buf[i] = bytearr[3]
    shm.buf[i + 1] = bytearr[2]
    shm.buf[i + 2] = bytearr[1]
    shm.buf[i + 3] = bytearr[0]


def WriteSelectedAction(action: int):
    # create message type
    # currently we only have one outgoing message, but this gives options to create more in the future
    InsertByteArrIntoBuffer(4, IntToBytes(1))

    # set selected action type
    InsertByteArrIntoBuffer(8, IntToBytes(int(action)))

    # set availability
    InsertByteArrIntoBuffer(0, IntToBytes(2))


parser = argparse.ArgumentParser()
parser.add_argument(
    "--no-train", action="store_true", help="Whether to disable training."
)
parser.add_argument(
    "--inference-mode", type=str, default="local", choices=["local", "remote"]
)
parser.add_argument(
    "--off-policy",
    action="store_true",
    help="Whether to compute random actions instead of on-policy "
    "(Policy-computed) ones.",
)
parser.add_argument(
    "--port", type=int, default=9900, help="The port to use (on localhost)."
)


if __name__ == "__main__":
    args = parser.parse_args()
    client = PolicyClient(f"http://localhost:{args.port}", inference_mode=args.inference_mode)
    eid = client.start_episode(training_enabled=not args.no_train)

    WaitForSharedMemStart()

    WaitForAvailability()
    states, _, _ = ReadStateRewardDoneFromBuffer()
    # print("len(states) == "+str(len(states)))
    # ReadBuffer()
    ClearBuffer()

    info = {}  # not implemented

    rewards = 0.0
    # print("before loop")
    while True:
        if args.off_policy:
            raise NotImplementedError("Off-Policy training is not implemented")
        else:
            # print("Getting action...")
            # print("\ttype: "+str(type(states)))
            # print("\tlen: " + str(len(states)))
            # ("\tstates: " + str(states))
            action = client.get_action(eid, states)

        # print("got action: "+str(action))
        WriteSelectedAction(action)
        WaitForAvailability()
        # print("Waiting for availability...")
        states, reward, done = ReadStateRewardDoneFromBuffer()
        ClearBuffer()
        # print("StateRewardDone received")

        # print("states: "+str(states))
        # print("len(states): " + str(len(states)))

        rewards += reward

        client.log_returns(eid, reward, info=info)

        if done == 1:
            print("Total Episode Reward: "+str(rewards))
            rewards = 0.0

            # End the old episode
            client.end_episode(eid, states)

            # Start a new episode
            eid = client.start_episode(training_enabled=not args.no_train)

