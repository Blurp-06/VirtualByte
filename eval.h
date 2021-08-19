#ifndef VIRTUAL_BYTE_EVAL
#define VIRTUAL_BYTE_EVAL
#include <map>
#include <vector>
#include <string>
#include "tools.h"
#include "parser.h"
#include "virtualMemory.h"
#include "executor.h"
#include <conio.h>

int virtualByteEval(ParsedLine line);
void initActions();
unsigned int getLinesEvaled();

#endif // !VIRTUAL_BYTE_EVAL