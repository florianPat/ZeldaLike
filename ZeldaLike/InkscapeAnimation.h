#pragma once

#include "InscapeAnimationElement.h"
#include "Animation.h"

//NOTE: Get rect from id to keyFrame
class InkscapeAnimation : public Animation
{
	InkscapeAnimationElement iae;
	std::vector<std::map<std::string, sf::IntRect>> inkscapeKeyFrames;
private:
	void setupInkscapeKeyFrames(const std::vector<std::string>& regionNames);
public:
	InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas& atlas, const std::string& inkscapeFileName, sf::Int64 frameDuration = sf::seconds(0.2f).asMicroseconds(), PlayMode type = PlayMode::LOOPED);
	InkscapeAnimation(const std::vector<std::string>& regionNames, const TextureAtlas& atlas, const InkscapeAnimationElement& iae, sf::Int64 frameDuration = sf::seconds(0.2f).asMicroseconds(), PlayMode type = PlayMode::LOOPED);
	//NOTE: Should only be used if Animation::SetKeyFrames() was used before
	void setInkscapeAnimationElement(const std::string& inkscapeFileName, const std::vector<std::string>& regionNames);
	sf::IntRect getInkscapeAnimationElementKeyFrame(const std::string& keyFrameId) const;
};