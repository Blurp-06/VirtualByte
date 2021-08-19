#include "eval.h"
#define REGISTER_ACTION(x) actions[#x] = &x
#define ARGUMENT_SIZE_CHECK(x) if(x < args.size()){throwError("Too many arguments", linesEvaled);}else if(x > args.size()){throwError("Not enough arguments", linesEvaled);}

static std::map<std::string, void(*)(std::vector<int>)> actions;
static unsigned int linesEvaled = 0;
static int dotValue = 0;

unsigned int getLinesEvaled()
{
	return linesEvaled;
}
void virtualByteEval(ParsedLine line)
{
	void(*function)(std::vector<int>);
	linesEvaled++;

	if (line.isEmpty)
	{
		return;
	}

	if (line.dotPos != -1)
	{
		line.args[line.dotPos] = dotValue;
	}

	try
	{
		function = actions.at(line.action);
		function(line.args);
	}
	catch (const std::exception&)
	{
		throwError("No action named " + line.action, linesEvaled);
	}
}

// Math actions.
void add(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(2);
	dotValue = args[0] + args[1];
}
void sub(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(2);
	dotValue = args[0] - args[1];
}

// Show commands (printing to the screen).
void show_str(std::vector<int> args)
{
	for (const int& arg : args)
	{
		std::cout << (unsigned char)arg;
	}
}
void show_int(std::vector<int> args)
{
	for (const int& arg : args)
	{
		std::cout << arg;
	}
}
void show_newline(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(0);
	std::cout << std::endl;
}
void show_mem_str(std::vector<int> args)
{
	for (const int& i : args)
	{
		std::cout << (unsigned char)getMemory(args[i]);
	}
}
void show_mem_int(std::vector<int> args)
{
	for (const int& i : args)
	{
		std::cout << getMemory(args[i]);
	}
}

// Memory operations.
void mem_init(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(1);
	initMemory(args[0]);
}
void mem_destruct(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(0);
	destructMemory();
}
void mem_dot(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(1);
	unsigned int index = args[0];
	dotValue = getMemory(index);
}
void mem_set(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(2);
	setMemory(args[0], args[1]);
}
void mem_add(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(3);
	int value = getMemory(args[0]) + getMemory(args[1]);
	setMemory(args[2], value);
}
void mem_sub(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(3);
	int value = getMemory(args[0]) - getMemory(args[1]);
	setMemory(args[2], value);
}

// Pauses the program.
void pause(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(0);
	std::cin.get();
}

// Quits the program.
void quit(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(1);
	exit(args.at(0));
}


void initActions()
{
	REGISTER_ACTION(add);
	REGISTER_ACTION(sub);
	REGISTER_ACTION(show_str);
	REGISTER_ACTION(show_int);
	REGISTER_ACTION(show_newline);
	REGISTER_ACTION(show_mem_str);
	REGISTER_ACTION(show_mem_int);
	REGISTER_ACTION(mem_init);
	REGISTER_ACTION(mem_destruct);
	REGISTER_ACTION(mem_dot);
	REGISTER_ACTION(mem_set);
	REGISTER_ACTION(mem_add);
	REGISTER_ACTION(mem_sub);
	REGISTER_ACTION(pause);
	REGISTER_ACTION(quit);
}