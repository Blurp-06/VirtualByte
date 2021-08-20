#include <iostream>
#include <fstream>
#include "parser.h"
#include "tools.h"
#include "eval.h"
#include "executor.h"

int main(int argc, char* argv[])
{
	// Getting file name.
#ifndef DEBUG
	if (argc != 2)
	{
		throwError("Incorrect amount of arguments, only expected file name.", 0);
	}
#endif // !DEBUG

	
	// Preparing for execution
	// parsing file.
	std::vector<ParsedLine> lines;
	
#ifdef DEBUG
	std::ifstream file("main.virb");
#else
	std::ifstream file(argv[1]);
#endif // DEBUG


	for (std::string currentLineString; std::getline(file, currentLineString);)
	{
		ParsedLine tmpLine = parseLine(currentLineString);

		if(tmpLine.hasError)
		{
			throwError(tmpLine.errorMessage, getLinesParsed());
		}
		lines.push_back(tmpLine);
	}

	startExecutor(lines);
}