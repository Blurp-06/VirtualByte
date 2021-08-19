#ifndef VIRTUAL_BYTE_EXECUTOR
#define VIRTUAL_BYTE_EXECUTOR
#include "parser.h"
#include "eval.h"
#include <map>
#include <vector>
#include "tools.h"

void startExecutor(std::vector<ParsedLine> lines);
int getCurrentLine();
void gotoLine(unsigned int line);

#endif // !VIRTUAL_BYTE_EXECUTOR