#include <iostream>
#include <fstream>
#include "parser.h"
#include "tools.h"
#include "eval.h"

int main(int argc, char* argv[])
{
	// Getting file name.
	if (argc != 2)
	{
		throwError("Incorrect amount of arguments, only expected file name.", 0);
	}
	
	// Preparing for execution
	// parsing file.
	std::vector<ParsedLine> lines;
	std::ifstream file(argv[1]);

	for (std::string currentLineString; std::getline(file, currentLineString);)
	{
		ParsedLine tmpLine = parseLine(currentLineString);

		if(tmpLine.hasError)
		{
			throwError(tmpLine.errorMessage, getLinesParsed());
		}
		lines.push_back(tmpLine);
	}

	// Running the code.
	initActions();
	for (ParsedLine line : lines)
	{
		virtualByteEval(line);
	}
}