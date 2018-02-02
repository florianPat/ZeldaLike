#include "Utils.h"

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
	return DelegateFunction(std::pair<int, std::function<void(EventData*)>>(getGUID(), function));
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
