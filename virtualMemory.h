#ifndef VIRTUAL_BYTE_VIRTUAL_MEMORY
#define VIRTUAL_BYTE_VIRTUAL_MEMORY
#include "tools.h"
#include "eval.h"
void initMemory(const unsigned int& size);
int getMemory(const unsigned int& index);
void setMemory(const unsigned int& index, const int& value);
void destructMemory();
#endif // !VIRTUAL_BYTE_VIRTUAL_MEMORY