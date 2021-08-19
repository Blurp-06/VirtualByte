#include "executor.h"

static int currentLine = 0;
int getCurrentLine()
{
	return currentLine;
}
void gotoLine(unsigned int line)
{
	currentLine = line;
}

void startExecutor(std::vector<ParsedLine> lines)
{
	// Running the code.
	initActions();
	while (currentLine != lines.size())
	{
		ParsedLine line = lines[currentLine];
		int newIndex = virtualByteEval(line);
		
		if (newIndex > lines.size())
		{
			throwError("Line not in file.", currentLine);
		}

		currentLine = newIndex;
	}
}