#include "virtualMemory.h"

static int* memory;
static bool isInitialized = false;
static unsigned int memorySize;

void initMemory(const unsigned int& size)
{
	if (isInitialized)
	{
		throwError("Memory already initialized.", getLinesEvaled());
	}
	memory = new int[size];
	for (int i = 0; i < size;i++)
	{
		memory[i] = 0;
	}
	memorySize = size;
	isInitialized = true;

}
int getMemory(const unsigned int& index)
{
	if (index >= memorySize)
	{
		throwError("Memory out of bound.", getLinesEvaled());
	}
	else if (!isInitialized)
	{
		throwError("Memory not initialized.", getLinesEvaled());
	}
	
	return memory[index];
}
void setMemory(const unsigned int& index, const int& value)
{
	if (index >= memorySize)
	{
		throwError("Memory out of bound.", getLinesEvaled());
	}
	else if (!isInitialized)
	{
		throwError("Memory not initialized.", getLinesEvaled());
	}

	memory[index] = value;
}
void destructMemory()
{
	delete[] memory;
	isInitialized = false;
}