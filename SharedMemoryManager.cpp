#include "SharedMemoryManager.h"

// Fixing error LNK2001: unresolved external symbol with these declarations
const char* SharedMemoryManager::SHARED_MEM_STR;
mapped_region* SharedMemoryManager::region;
windows_shared_memory* SharedMemoryManager::shm;
unsigned int SharedMemoryManager::currentBufferCount;
bool SharedMemoryManager::firstAction;
// -------------------------------------------------------

unsigned int const NUMBER_OF_BYTES_IN_INT = 4; // floats are also 4 bytes so using this CONST for those as well
unsigned int const ENV_AVAILABILITY = 1; // write byte that indicates message from environment is available.

void SharedMemoryManager::Init(windows_shared_memory* smh, mapped_region* region)
{
	SHARED_MEM_STR = "PSEMem";
	currentBufferCount = 0;
	firstAction = true;

	SharedMemoryManager::shm = smh;
	SharedMemoryManager::region = region;
}

void SharedMemoryManager::AddIntToBuffer(int val)
{
	if (currentBufferCount + 1 > region->get_size() / NUMBER_OF_BYTES_IN_INT) // +1 to leave space in beggining for 'availability' flag
		throw std::runtime_error("Buffer is full but trying to add more values");

	void* ptr = OffsetPtr(region->get_address(), (NUMBER_OF_BYTES_IN_INT * currentBufferCount) + NUMBER_OF_BYTES_IN_INT);

	//void* ptr = region->get_address();
	//ptr = static_cast<char*>(ptr) + (NUMBER_OF_BYTES_IN_INT * currentBufferCount) + NUMBER_OF_BYTES_IN_INT; // additional 4 bytes in beginning is left for 'availability' flag

	std::memcpy(ptr, &val, NUMBER_OF_BYTES_IN_INT);

	currentBufferCount++;
}

void SharedMemoryManager::AddFloatToBuffer(float val)
{
	if (currentBufferCount + 1 > region->get_size() / NUMBER_OF_BYTES_IN_INT) // +1 to leave space in beggining for 'availability' flag
		throw std::runtime_error("Buffer is full but trying to add more values");

	void* ptr = OffsetPtr(region->get_address(), (NUMBER_OF_BYTES_IN_INT * currentBufferCount) + NUMBER_OF_BYTES_IN_INT);

	//void* ptr = region->get_address();
	//ptr = static_cast<char*>(ptr) + (NUMBER_OF_BYTES_IN_INT * currentBufferCount) + NUMBER_OF_BYTES_IN_INT; // additional 4 bytes in beginning is left for 'availability' flag

	std::memcpy(ptr, &val, NUMBER_OF_BYTES_IN_INT);

	currentBufferCount++;
}

void SharedMemoryManager::SetAvailability(int val)
{
	std::memcpy(region->get_address(), &val, NUMBER_OF_BYTES_IN_INT);
}

void SharedMemoryManager::ClearBuffer()
{
	std::memset(region->get_address(), 0, region->get_size());
	currentBufferCount = 0;
}

void SharedMemoryManager::SendStateAndReward(float reward, std::vector<int> currentState)
{
	VerifyBufferIsClear();

	printf("SMM: Adding Reward - %f\n", reward);
	AddFloatToBuffer(reward);

	printf("SMM: Adding State Size - %d\n", currentState.size());
	AddIntToBuffer(currentState.size());

	int count = 0;
	while (!currentState.empty()) 
	{
		//printf("\tback() -> %d\n", currentState.back());
		AddIntToBuffer(currentState.back());
		currentState.pop_back();
		count++;
	}
	printf("Count: %d\n", count);

	// delete currentState; may or may not need this.

	SetAvailability(ENV_AVAILABILITY);
}

void SharedMemoryManager::WaitForAvailability() 
{
	while (true) 
	{
		int val;
		std::memcpy(&val, region->get_address(), NUMBER_OF_BYTES_IN_INT);

		if (val == 2)
			break;
	}
}

ActionType SharedMemoryManager::ReadSelectedAction() 
{
	int selectedAction;
	if (!firstAction) {
		selectedAction = ReadIntFromPtr(OffsetPtr(region->get_address(), NUMBER_OF_BYTES_IN_INT * 2));
	}
	else 
	{
		selectedAction = 0;
		firstAction = false;
	}
	return static_cast<ActionType>(selectedAction);
}

void SharedMemoryManager::VerifyBufferIsClear()
{
	if (currentBufferCount != 0)
		throw std::logic_error("currentBufferCount != 0, buffer still may contain data and not be cleared");

	int availability;
	std::memcpy(&availability, region->get_address(), NUMBER_OF_BYTES_IN_INT);
	if (availability != 0)
		throw std::logic_error("Availability byte is not 0, buffer still contains data and has not been cleared.");
}

int SharedMemoryManager::ReadIntFromPtr(void* ptr)
{
	int val;
	std::memcpy(&val, ptr, NUMBER_OF_BYTES_IN_INT);
	return val;
}

void* SharedMemoryManager::OffsetPtr(void* ptr, int i) 
{
	return static_cast<char*>(ptr) + i; // additional 4 bytes in beginning is left for 'availability' flag
}
