#include "eval.h"
#define REGISTER_ACTION(x) actions[#x] = &x
#define ARGUMENT_SIZE_CHECK(x) if(x < args.size()){throwError("Too many arguments", linesEvaled);}else if(x > args.size()){throwError("Not enough arguments", linesEvaled);}

static std::map<std::string, void(*)(std::vector<int>)> actions;
static std::map<std::string, void(*)(int[], int*)> dllActions;
static unsigned int linesEvaled = 0;
static int dotValue = 0;
static std::map<int, int> labels;
static std::vector<HINSTANCE> openDll;

static void setDot(int value)
{
	dotValue = value;
}
static int getDot()
{
	return dotValue;
}
unsigned int getLinesEvaled()
{
	return linesEvaled;
}
int virtualByteEval(ParsedLine line)
{
	void(*function)(std::vector<int>);
	linesEvaled++;

	if (line.isEmpty)
	{
		return getCurrentLine() + 1;
	}

	// Setting dot.
	if (line.dotPos != -1)
	{
		line.args[line.dotPos] = getDot();
	}

	try
	{
		if (line.action[0] == '#')
		{
			void(*f)(int[], int*) = dllActions.at(line.action);
			int* args = new int[line.args.size()];

			for (unsigned int i = 0; i < line.args.size(); i++)
			{
#pragma warning(disable: 6386)
				args[i] = line.args[i];
#pragma warning(default: 6386)
			}

			f(args, &dotValue);
			delete[] args;
		}
		else
		{
			function = actions.at(line.action);
			function(line.args);
		}
	}
	catch (const std::exception&)
	{
		throwError("No action named " + line.action, linesEvaled);
	}

	return getCurrentLine() + 1;
}

// Recursion.
void label(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(1);
	labels[args[0]] = getCurrentLine();
}
void jmp(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(1);

	if (labels.count(args[0]) == 0)
	{
		throwError("Label not found.", getCurrentLine() + 1);
	}

	gotoLine(labels[args[0]]);
}
void jmp_if(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(3);

	if (labels.count(args[0]) == 0)
	{
		throwError("Label not found.", getCurrentLine() + 1);
	}

	if (args[1] == args[2])
	{
		gotoLine(labels[args[0]]);
	}
}
void jmp_ifn(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(3);

	if (labels.count(args[0]) == 0)
	{
		throwError("Label not found.", getCurrentLine() + 1);
	}

	if (args[1] != args[2])
	{
		gotoLine(labels[args[0]]);
	}
}
void jmp_skip(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(1);
	
	gotoLine(getCurrentLine() + args[0]);
}
void jmp_skip_if(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(3);

	if (args[1] == args[2])
	{
		gotoLine(getCurrentLine() + args[0]);
	}
}
void jmp_skip_ifn(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(3);

	if (args[1] != args[2])
	{
		gotoLine(getCurrentLine() + args[0]);
	}
}

// Math actions.
void add(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(2);
	setDot(args[0] + args[1]);
}
void sub(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(2);
	setDot(args[0] - args[1]);
}

// Dot operations.
void dot_clear(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(0);
	dotValue = 0;
}
void dot_set(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(1);
	setDot(args[0]);
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

// Get commands (gets user input).
void get_char(std::vector<int> args)
{
	ARGUMENT_SIZE_CHECK(0);
	char c = _getch();
	setDot((int)c);
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

// Dll support.
void get_dll(std::vector<int> args)
{
	std::string name;
	for (int arg : args)
	{
		name.push_back((char)arg);
	}
	typedef const char*(__stdcall* init_func_t)();
	HINSTANCE hGetProcIDDLL = LoadLibraryA(name.c_str());

	if (!hGetProcIDDLL)
	{
		throwError("Unable to load dll: " + name, getCurrentLine());
	}

	// Resolve function address here.
	init_func_t initFunc = (init_func_t)GetProcAddress(hGetProcIDDLL, "initActionsExt");
	if (!initFunc)
	{
		throwError("Could not load init function.", getCurrentLine());
	}

	// Getting all the function names.
	const char* funcNames = initFunc();
	std::vector<std::string> serialized;
	std::string toPush;

	for (unsigned int i = 0; i < strlen(funcNames); i++)
	{
		char c = funcNames[i];
		if (c == '|')
		{
			serialized.push_back(toPush);
			toPush = "";
			continue;
		}
		toPush.push_back(c);
	}

	serialized.push_back(toPush);

	// Getting the functions.
	typedef void (__stdcall* call_func_t)(int[], int*);
	for (std::string s : serialized)
	{
		call_func_t callFunc = (call_func_t)GetProcAddress(hGetProcIDDLL, s.c_str());
		if (!callFunc)
		{
			throwError("Could not load function from dll: " + s, getCurrentLine());
		}
		dllActions["#" + s] = (void(*)(int[], int*))callFunc;
	}

	openDll.push_back(hGetProcIDDLL);
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
	for (HINSTANCE dll : openDll)
	{
		FreeLibrary(dll);
	}
	exit(args.at(0));
}


void initActions()
{
	// Build-in actions.
	REGISTER_ACTION(label);
	REGISTER_ACTION(jmp);
	REGISTER_ACTION(jmp_if);
	REGISTER_ACTION(jmp_ifn);
	REGISTER_ACTION(jmp_skip);
	REGISTER_ACTION(jmp_skip_if);
	REGISTER_ACTION(jmp_skip_ifn);
	REGISTER_ACTION(add);
	REGISTER_ACTION(sub);
	REGISTER_ACTION(dot_clear);
	REGISTER_ACTION(dot_set);
	REGISTER_ACTION(show_str);
	REGISTER_ACTION(show_int);
	REGISTER_ACTION(show_newline);
	REGISTER_ACTION(show_mem_str);
	REGISTER_ACTION(show_mem_int);
	REGISTER_ACTION(get_char);
	REGISTER_ACTION(mem_init);
	REGISTER_ACTION(mem_destruct);
	REGISTER_ACTION(mem_dot);
	REGISTER_ACTION(mem_set);
	REGISTER_ACTION(mem_add);
	REGISTER_ACTION(mem_sub);
	REGISTER_ACTION(get_dll);
	REGISTER_ACTION(pause);
	REGISTER_ACTION(quit);
}