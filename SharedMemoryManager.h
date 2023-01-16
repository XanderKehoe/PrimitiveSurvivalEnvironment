#pragma once
#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "ActionType.h"

using namespace boost::interprocess;

class SharedMemoryManager
{
public:
	static void Init(windows_shared_memory* smh, mapped_region* region);
	static void AddIntToBuffer(int val);
	static void AddFloatToBuffer(float val);
	static void SetAvailability(int val);
	static void ClearBuffer();
	static void SendStateRewardDone(float reward, int done, std::vector<int> currentIntState, std::vector<float> currentFloatState); // Sends the current state, the reward, and any additional info from this time step.
	static void SendTestData();
	static ActionType ReadSelectedAction();
	static void WaitForAvailability();
private:
	static const char* SHARED_MEM_STR;
	static mapped_region* region;
	static windows_shared_memory* shm;

	unsigned static int currentBufferCount;
	static bool firstAction;

	static void VerifyBufferIsClear();
	static int ReadIntFromPtr(void* ptr);
	static void* OffsetPtr(void* ptr, int i);
};

