#ifndef VIRTUAL_BYTE_PARSER
#define VIRTUAL_BYTE_PARSER
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

struct ParsedLine;
unsigned int getLinesParsed();
ParsedLine parseLine(std::string line);

struct ParsedLine
{
	std::string action;
	std::vector<int> args;
	std::string errorMessage;
	bool hasError = false;
	bool hasArgs;
	bool isEmpty;
	int dotPos = -1;
};
#endif // VIRTUAL_BYTE_PARSER