#pragma once

#include <string>
#include <vector>
#include "TextureAtlas.h"
#include "Animation.h"
#include <iostream>

#define utilsLog(string) std::cerr << "Log: Function: " << __FUNCTION__ << ", Line: " << __LINE__ << ", Message: " << string << std::endl

namespace utils
{
	bool isWordInLine(const std::string & word, const std::string & lineContent);
	void addAnimation(std::vector<std::string> regionNames, std::string animationName, TextureAtlas& atlas, std::map<std::string, Animation>& animations);
}