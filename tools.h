#ifndef VIRTUAL_BYTE_TOOLS
#define VIRTUAL_BYTE_TOOLS
#include "parser.h"
#include <iostream>
// Function will print out some info about a line class.
void printLine(const ParsedLine& l);
void throwError(const std::string& message, const unsigned int& lineNumber);
#endif // !VIRTUAL_BYTE_TOOLS