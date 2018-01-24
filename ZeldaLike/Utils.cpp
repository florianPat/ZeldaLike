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

void utils::addAnimation(std::vector<std::string> regionNames, std::string animationName, TextureAtlas& atlas, std::map<std::string, Animation>& animations)
{
	std::vector<TextureRegion> regions;
	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		auto region = atlas.findRegion(*it);
		if (region == nullptr)
			continue;
		else
		{
			regions.push_back(*region);
		}
	}

	animations.emplace(animationName, Animation(regions, sf::seconds(0.2f).asMicroseconds(), Animation::PlayMode::LOOPED));
}

DelegateFunction utils::getDelegateFromFunction(std::function<void(EventData*)>& function)
{
	return DelegateFunction(std::pair<int, std::function<void(EventData*)>>(getGUID(), function));
}

#pragma comment(lib, "rpcrt4.lib")  // UuidCreate
#include <Windows.h>

const unsigned int utils::getGUID()
{
	UUID result;
	UuidCreate(&result);
	return result.Data3;
}