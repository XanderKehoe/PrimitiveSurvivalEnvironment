#include "SharedMemoryManager.h"

// Fixing error LNK2001: unresolved external symbol with these declarations
const char* SharedMemoryManager::SHARED_MEM_STR;
mapped_region* SharedMemoryManager::region;
windows_shared_memory* SharedMemoryManager::shm;
unsigned int SharedMemoryManager::currentBufferCount;
// -------------------------------------------------------

unsigned int const NUMBER_OF_BYTES_IN_INT = 4; // floats are also 4 bytes so using this CONST for those as well
unsigned int const ENV_AVAILABILITY = 1; // write byte that indicates message from environment is available.

void SharedMemoryManager::Init(windows_shared_memory* smh, mapped_region* region)
{
	SHARED_MEM_STR = "PSEMem";
	currentBufferCount = 0;

	SharedMemoryManager::shm = smh;
	SharedMemoryManager::region = region;
}

void SharedMemoryManager::AddIntToBuffer(int val)
{
	if (currentBufferCount + 1 > region->get_size() / NUMBER_OF_BYTES_IN_INT) // +1 to leave space in beggining for 'availability' flag
		throw std::runtime_error("Buffer is full but trying to add more values");

	void* ptr = region->get_address();
	ptr = static_cast<char*>(ptr) + (NUMBER_OF_BYTES_IN_INT * currentBufferCount); // additional 4 bytes in beginning is left for 'availability' flag

	std::memcpy(ptr, &val, NUMBER_OF_BYTES_IN_INT);

	currentBufferCount++;
}

void SharedMemoryManager::AddFloatToBuffer(float val)
{
	if (currentBufferCount + 1 > region->get_size() / NUMBER_OF_BYTES_IN_INT) // +1 to leave space in beggining for 'availability' flag
		throw std::runtime_error("Buffer is full but trying to add more values");

	void* ptr = region->get_address();
	ptr = static_cast<char*>(ptr) + (NUMBER_OF_BYTES_IN_INT * currentBufferCount); // additional 4 bytes in beginning is left for 'availability' flag

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

void SharedMemoryManager::SendStateAndReward(int reward, std::vector<float> currentState)
{
	VerifyBufferIsClear();

	AddIntToBuffer(reward);

	AddIntToBuffer(currentState.size());

	while (!currentState.empty()) 
	{
		AddFloatToBuffer(currentState.back());
		currentState.pop_back();
	}

	SetAvailability(ENV_AVAILABILITY);
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
