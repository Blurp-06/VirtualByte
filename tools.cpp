#include "tools.h"

void printLine(const ParsedLine& l)
{
	std::cout << "-----------------------------" << std::endl;
	if (l.isEmpty) { std::cout << "Empty line." << std::endl; return; }
	std::cout << l.action << std::endl;
	if (l.hasError) { std::cout << l.errorMessage << std::endl; }
	for (int s : l.args) { std::cout << s << std::endl; }
}
void throwError(const std::string& message, const unsigned int& lineNumber)
{
	std::cout << "Error on line " << lineNumber << ": " << message << std::endl;
	exit(-1);
}