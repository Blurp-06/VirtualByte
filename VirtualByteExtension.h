/*
Note for people making Virtual Byte extensions:
	The best way to make it is with vs-2019 and the dll project template.
	Once the project is created you add this file to the project.
	Then you make a cpp file that include's this header and then you define the initActionExt funtion.
	After that you can write functions.
*/

#pragma once
#define API __declspec(dllexport)
// Return a string with the names of the functions like this: "functionA|functionB"
// Don't add spaces or anything else, there should only be a '|' between function names.
// Remember to spell them correctly.
extern "C" API const char* initActionsExt();

// To add a function to VirtualByte do this:
// extern "C" API void functionA(int args[], int* dotValue);