#include "Animation.h"
#include <assert.h>
#include "Utils.h"
#include <fstream>

Animation::Animation(std::vector<TextureRegion>& keyFrames, sf::Int64 frameDuration, PlayMode type)
	: keyFrames(), frameDuration(frameDuration), playMode(type), clock(), keyFrameIt(), keyFrameItReverse()
{
	for (auto it = keyFrames.begin(); it != keyFrames.end(); ++it)
	{
		this->keyFrames.push_back(it->getRegion());
	}

	keyFrameIt = 0;
	keyFrameItReverse = this->keyFrames.size() - 1;
}

Animation::Animation(const std::vector<std::string>& regionNames, const TextureAtlas & atlas, sf::Int64 frameDuration, PlayMode type)
	: keyFrames(), frameDuration(frameDuration), playMode(type), clock(), keyFrameIt(), keyFrameItReverse()
{
	std::vector<TextureRegion> keyFrames;
	for (auto it = regionNames.begin(); it != regionNames.end(); ++it)
	{
		auto region = atlas.findRegion(*it);
		if (region == nullptr)
			continue;
		else
		{
			keyFrames.push_back(*region);
		}
	}

	for (auto it = keyFrames.begin(); it != keyFrames.end(); ++it)
	{
		this->keyFrames.push_back(it->getRegion());
	}

	keyFrameIt = 0;
	keyFrameItReverse = this->keyFrames.size() - 1;
}

Animation::PlayMode Animation::getPlayMode()
{
	return playMode;
}

sf::Int64 Animation::getFrameDuration()
{
	return frameDuration;
}

void Animation::setFrameDuration(sf::Int64 newFrameDuration)
{
	if (newFrameDuration > 0)
	{
		frameDuration = newFrameDuration;
	}
}

void Animation::setKeyFrames(std::vector<TextureRegion>& newKeyFrames)
{
	keyFrames.clear();
	for (auto it = newKeyFrames.begin(); it != newKeyFrames.end(); ++it)
	{
		keyFrames.push_back(it->getRegion());
	}
}

bool Animation::isAnimationFinished()
{
	if (!paused)
	{
		assert(playMode == Animation::PlayMode::NORMAL || playMode == Animation::PlayMode::REVERSED);

		currentTime += clock.restart().asMicroseconds();

		if (playMode == Animation::PlayMode::NORMAL)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (++keyFrameIt == keyFrames.size())
				{
					keyFrameIt = keyFrames.size() - 1;
					return true;
				}
			}

			return false;
		}
		else if (playMode == Animation::PlayMode::REVERSED)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (--keyFrameItReverse == 0)
				{
					keyFrameItReverse = 0;
					return true;
				}
			}

			return false;
		}
		else
		{
			InvalidCodePath;
			return false;
		}
	}
	else
	{
		if (playMode == Animation::PlayMode::NORMAL)
		{
			if (++keyFrameIt == keyFrames.size())
			{
				keyFrameIt = keyFrames.size() - 1;
				return true;
			}
			else
				return false;
		}
		else if (playMode == Animation::PlayMode::REVERSED)
		{
			if (--keyFrameItReverse == -1)
			{
				keyFrameItReverse = 0;
				return true;
			}
			else
				return false;
		}
		else
		{
			InvalidCodePath;
			return false;
		}
	}
}

sf::Sprite Animation::getKeyFrame()
{
	if (!paused)
	{
		currentTime += clock.restart().asMicroseconds();

		if (playMode == Animation::PlayMode::NORMAL)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (++keyFrameIt == keyFrames.size())
				{
					keyFrameIt = keyFrames.size() - 1;
				}
			}

			return keyFrames[keyFrameIt];
		}
		else if (playMode == Animation::PlayMode::LOOPED)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (++keyFrameIt == keyFrames.size())
					keyFrameIt = 0;
			}

			return keyFrames[keyFrameIt];
		}
		else if (playMode == Animation::PlayMode::REVERSED)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (--keyFrameItReverse == -1)
				{
					++keyFrameItReverse;
					break;
				}
			}

			return keyFrames[keyFrameItReverse];
		}
		else if (playMode == Animation::PlayMode::LOOP_REVERSED)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (--keyFrameItReverse == -1)
					keyFrameItReverse = keyFrames.size() - 1;
			}

			return keyFrames[keyFrameItReverse];
		}
		else
		{
			if (playMode == Animation::PlayMode::NORMAL || playMode == Animation::PlayMode::LOOPED)
				return keyFrames[keyFrameIt];
			else if (playMode == Animation::PlayMode::REVERSED || playMode == Animation::PlayMode::LOOP_REVERSED)
				return keyFrames[keyFrameItReverse];
			else
				return keyFrames[0];
		}
	}
	else
	{
		if (playMode == Animation::PlayMode::NORMAL || playMode == Animation::PlayMode::LOOPED)
			return keyFrames[keyFrameIt];
		else if (playMode == Animation::PlayMode::REVERSED || playMode == Animation::PlayMode::LOOP_REVERSED)
			return keyFrames[keyFrameItReverse];
		else
		{
			InvalidCodePath;
			return keyFrames[0];
		}
	}
}

void Animation::setPlayMode(PlayMode & newPlayMode)
{
	playMode = newPlayMode;
	keyFrameIt = 0;
	keyFrameItReverse = keyFrames.size() - 1;
}

void Animation::restartFrameTimer()
{
	clock.restart();
}

void Animation::pause()
{
	paused = true;
}

void Animation::resume()
{
	paused = false;
	restartFrameTimer();
}

void Animation::restart()
{
	keyFrameIt = 0;
	keyFrameItReverse = keyFrames.size() - 1;
	restartFrameTimer();
}