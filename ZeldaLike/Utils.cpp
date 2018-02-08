#include "Utils.h"
#include <Windows.h>

#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000

unsigned int utils::getGUID()
{
	UUID uuid;
	UuidCreate(&uuid);
	return uuid.Data1;
}

bool utils::isWordInLine(const std::string & word, const std::string & lineContent)
{
	size_t o = 0;
	bool result = false;
	while (o < lineContent.size() && !result)
	{
		o = lineContent.find(word[0], o);
		std::string searchWord(word);
		auto it = ++searchWord.begin();
		for (; o < lineContent.size(); ++it)
		{
			if (it != searchWord.end())
			{
				if (lineContent.at(++o) == it[0])
					continue;
				else
					break;
			}
			else
			{
				result = true;
				break;
			}
		}
	}

	return result;
}

DelegateFunction utils::getDelegateFromFunction(std::function<void(EventData*)>& function)
{
	return DelegateFunction(std::pair<unsigned int, std::function<void(EventData*)>>(getGUID(), function));
}

std::string utils::getWordBetweenChars(const std::string& lineContent, char firstChar, char lastChar)
{
	std::string result;
	size_t first = lineContent.find_first_of(firstChar);
	++first;
	size_t last = lineContent.find_last_of(lastChar);
	result = lineContent.substr(first, last - first);
	return result;
}

float utils::lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}
