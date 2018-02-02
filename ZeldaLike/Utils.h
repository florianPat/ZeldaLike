#pragma once

#include <string>
#include <vector>
#include "TextureAtlas.h"
#include "Animation.h"
#include <iostream>
#include <assert.h>
#include "EventManager.h"

#define utilsLog(string) std::cerr << "Log: Function: " << __FUNCTION__ << ", Line: " << __LINE__ << ", Message: " << string << std::endl
#define utilsLogBreak(string) utilsLog(string); __debugbreak()

#define InvalidCodePath assert(!"InvalidCodePath")

#define Kilobyte(x) x * 1024ll
#define Megabyte(x) Kilobyte(x) * 1024ll
#define Gigabyte(x) Megabyte(x) * 1024ll

#define getGUID() __COUNTER__

namespace utils
{
	bool isWordInLine(const std::string & word, const std::string & lineContent);
	DelegateFunction getDelegateFromFunction(std::function<void(EventData*)>& function);
	std::string getWordBetweenChars(const std::string& lineContent, char first, char last);
}