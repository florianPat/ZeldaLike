#include "InscapeAnimationElement.h"
#include "Utils.h"
#include <fstream>

InkscapeAnimationElement::InkscapeAnimationElement(const std::string& inkscapeFileName, const std::vector<std::string>& regionNames) : elementMap()
{
	std::ifstream file(inkscapeFileName);
	assert(file.good());

	std::string lineContent;

	for(int iteration = 0; !file.eof(); ++iteration)
	{
		if (lineContent.find("</g>") != std::string::npos)
		{
			break;
		}
		else if (iteration == 1)
		{
			utilsLogBreak("There is an element outside a group!!");
		}

		std::getline(file, lineContent);

		while (FindGroupLayer(lineContent))
		{
			std::getline(file, lineContent);
		}

		for (std::getline(file, lineContent); lineContent.find("<g") != std::string::npos; std::getline(file, lineContent))
		{
			std::getline(file, lineContent); //<g
			std::string groupId;
			std::map<std::string, sf::IntRect> rectMap;
			sf::Vector2i translationVec = { 0, 0 };
			sf::Vector2i scalingVec = { 1, 1 };
			bool shouldAdd = true;
			for (; shouldAdd && lineContent.find("</g>") == std::string::npos; std::getline(file, lineContent))
			{
				if (lineContent.find("id") != std::string::npos)
				{
					groupId = utils::getWordBetweenChars(lineContent, '"', '"');
					if (!regionNames.empty())
					{
						if (regionNames[0] == "Process all groups")
							continue;
					}
					for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
					{
						if (groupId.find(*it) == std::string::npos)
						{
							shouldAdd = false;
							break;
						}
					}
				}
				else if (lineContent.find("<rect") != std::string::npos)
				{
					std::string id;
					sf::IntRect rect;
					bool shouldAdd = true;
					//TODO: Think about how to make this nicer!
					bool addWidth = false;

					sf::Vector2i beforeTranslationVec = translationVec;
					sf::Vector2i beforeScalingVec = scalingVec;

					for (std::getline(file, lineContent); shouldAdd; std::getline(file, lineContent))
					{
						if (lineContent.find("x") != std::string::npos)
						{
							rect.left = atoi(utils::getWordBetweenChars(lineContent, '"', '"').c_str());
						}
						else if (lineContent.find("y") != std::string::npos)
						{
							if(lineContent.find("r") == std::string::npos)
								rect.top = atoi(utils::getWordBetweenChars(lineContent, '"', '"').c_str());
						}
						else if (lineContent.find("width") != std::string::npos)
						{
							rect.width = atoi(utils::getWordBetweenChars(lineContent, '"', '"').c_str());
							if (addWidth)
								translationVec.x += -rect.width;
						}
						else if (lineContent.find("height") != std::string::npos)
						{
							rect.height = atoi(utils::getWordBetweenChars(lineContent, '"', '"').c_str());
						}
						else if (lineContent.find("id") != std::string::npos)
						{
							id = utils::getWordBetweenChars(lineContent, '"', '"');
							if (id.find("rect") != std::string::npos || id.find("-") != std::string::npos)
							{
								shouldAdd = false;
								break;
							}
							size_t underlinePos = id.find_first_of('_');
							if (underlinePos != std::string::npos)
							{
								id = id.substr(0, underlinePos);
							}
						}
						else if (lineContent.find("transform") != std::string::npos)
						{
							if (lineContent.find("scale") != std::string::npos)
							{
								auto firstOne = lineContent.find_first_of('1');
								auto lasttOne = lineContent.find_last_of('1');
								if (firstOne == lasttOne)
								{
									utilsLogBreak("Scaling is only allowed with a 1");
								}
								else
								{
									bool isXScale = atoi(utils::getWordBetweenChars(lineContent, '(', ',').c_str()) == -1 ? true : false;
									if (isXScale)
									{
										scalingVec.x = -1;
										if (rect.width != 0)
											translationVec.x += -rect.width;
										else
											addWidth = true;
									}
									else
									{
										//TODO: Same as in x?
										utilsLogBreak("-1 scaling in Y is currently not implemented");
									}
								}
							}
							else if (lineContent.find("translate") != std::string::npos)
							{
								translationVec.x += atoi(utils::getWordBetweenChars(lineContent, '(', ',').c_str());
								translationVec.y += atoi(utils::getWordBetweenChars(lineContent, ',', ')').c_str());
							}
							else
							{
								utilsLogBreak("Other transformations are currently not supported");
							}
						}

						if (lineContent.find("/>") != std::string::npos)
						{
							break;
						}
					}
					if (shouldAdd)
					{
						rect.left *= scalingVec.x;
						rect.top *= scalingVec.y;
						rect.left += translationVec.x;
						rect.top += translationVec.y;
						rectMap.emplace(id, rect);

						scalingVec = beforeScalingVec;
						translationVec = beforeTranslationVec;
					}
					else
					{
						scalingVec = beforeScalingVec;
						translationVec = beforeTranslationVec;

						for (; lineContent.find("/>") == std::string::npos; std::getline(file, lineContent));
					}
				}
				else if (lineContent.find("<") != std::string::npos)
				{
					for (std::getline(file, lineContent); lineContent.find("/>") == std::string::npos; std::getline(file, lineContent));
				}
				else if (lineContent.find("transform") != std::string::npos)
				{
					if (lineContent.find("translate") != std::string::npos)
					{
						translationVec.x += atoi(utils::getWordBetweenChars(lineContent, '(', ',').c_str());
						translationVec.y += atoi(utils::getWordBetweenChars(lineContent, ',', ')').c_str());
					}
					else
					{
						utilsLogBreak("Other transformations are currently not supported");
					}
				}
			}

			for (; lineContent.find("</g>") == std::string::npos; std::getline(file, lineContent));

			if (!rectMap.empty())
			{
				auto base = rectMap.find("Base");
				if (base != rectMap.end())
				{
					if (rectMap.size() == 1)
					{
						utilsLogBreak("Only found \"Base\" element, but there is need for more!");
					}

					sf::Vector2i baseVec = { base->second.left, base->second.top };
					rectMap.erase(base);
					for (auto it = rectMap.begin(); it != rectMap.end(); ++it)
					{
						assert(baseVec.x <= it->second.left);
						assert(baseVec.y <= it->second.top);

						it->second.left -= baseVec.x;
						it->second.top -= baseVec.y;

						//NOTE: Hard coded 64 because thats my image size
						assert(it->second.left <= 64 && it->second.left >= 0);
						assert(it->second.top <= 64 && it->second.top >= 0);
					}
				}
				else
				{
					utilsLogBreak("No \"Base\" found in this rect definition");
				}

				elementMap.emplace(groupId, rectMap);
			}
		}
	}
}

InkscapeAnimationElement::InkscapeAnimationElement(const std::string & inkscapeFileName) : InkscapeAnimationElement(inkscapeFileName, { "Process all groups" })
{
}

bool InkscapeAnimationElement::FindGroupLayer(std::string & lineContent) const
{
	if (lineContent.find(std::string("id=\"layer")) == std::string::npos)
		return true;
	else
		return false;
}

sf::IntRect InkscapeAnimationElement::getElementRect(std::string& keyFrameId, std::string& elementId) const
{
	auto keyFrameResult = elementMap.find(keyFrameId);
	if (keyFrameResult != elementMap.end())
	{
		auto result = keyFrameResult->second.find(elementId);
		if (result != keyFrameResult->second.end())
			return result->second;
		else
		{
			InvalidCodePath;
			return sf::IntRect();
		}
	}
	else
	{
		InvalidCodePath;
		return sf::IntRect();
	}
}

std::map<std::string, sf::IntRect> InkscapeAnimationElement::getElementMap(const std::string & keyFrameId) const
{
	auto result = elementMap.find(keyFrameId);
	if (result != elementMap.end())
		return result->second;
	else
	{
		InvalidCodePath;
		return std::map<std::string, sf::IntRect>();
	}
}

bool InkscapeAnimationElement::isElementInMap(const std::string & keyFrameId) const
{
	auto result = elementMap.find(keyFrameId);
	return result != elementMap.end();
}
