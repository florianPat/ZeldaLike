#include "InkscapeAnimation.h"
#include "Utils.h"

void InkscapeAnimation::setupInkscapeKeyFrames(const std::vector<std::string>& regionNames)
{
	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		if (iae.isElementInMap(*it))
			inkscapeKeyFrames.push_back(std::map<std::string, sf::IntRect>{ iae.getElementMap(*it) });
	}
}

InkscapeAnimation::InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas & atlas, const std::string& inkscapeFileName, sf::Int64 frameDuration, PlayMode type)
	: iae(inkscapeFileName, regionNames), inkscapeKeyFrames(), Animation(regionNames, atlas, frameDuration, type)
{
	setupInkscapeKeyFrames(regionNames);
}

InkscapeAnimation::InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas & atlas, const InkscapeAnimationElement & iae, sf::Int64 frameDuration, PlayMode type)
	: iae(iae), inkscapeKeyFrames(), Animation(regionNames, atlas, frameDuration, type)
{
	setupInkscapeKeyFrames(regionNames);
}

void InkscapeAnimation::setInkscapeAnimationElement(const std::string& inkscapeFileName, const std::vector<std::string>& regionNames)
{
	inkscapeKeyFrames.clear();

	iae = InkscapeAnimationElement(inkscapeFileName, regionNames);

	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		inkscapeKeyFrames.push_back(std::map<std::string, sf::IntRect>{ iae.getElementMap(*it) });
	}
}

sf::IntRect InkscapeAnimation::getInkscapeAnimationElementKeyFrame(const std::string & keyFrameId) const
{
	size_t i = (playMode == PlayMode::LOOPED || playMode == PlayMode::NORMAL) ? keyFrameIt : keyFrameItReverse;

	auto result = inkscapeKeyFrames[i].find(keyFrameId);
	if (result != inkscapeKeyFrames[i].end())
		return result->second;
	else
	{
		InvalidCodePath;
		return sf::IntRect();
	}
}
