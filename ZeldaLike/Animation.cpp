#include "Animation.h"
#include <assert.h>
#include "Utils.h"

Animation::Animation(std::vector<TextureRegion>& keyFrames, sf::Int64 frameDuration, PlayMode type) : keyFrames(),
	frameDuration(frameDuration), playMode(type), clock(), keyFrameIt(), keyFrameItReverse()
{
	for (auto it = keyFrames.begin(); it != keyFrames.end(); ++it)
	{
		this->keyFrames.push_back(it->getRegion());
	}

	keyFrameIt = this->keyFrames.begin();
	keyFrameItReverse = this->keyFrames.rbegin();
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
				if (++keyFrameIt == keyFrames.end())
				{
					keyFrameIt = --keyFrames.end();
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
				if (++keyFrameItReverse == keyFrames.rend())
				{
					--keyFrameItReverse;
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
			if (++keyFrameIt == keyFrames.end())
			{
				--keyFrameIt;
				return true;
			}
			else
				return false;
		}
		else if (playMode == Animation::PlayMode::REVERSED)
		{
			if (++keyFrameItReverse == keyFrames.rend())
			{
				--keyFrameItReverse;
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
				if (++keyFrameIt == keyFrames.end())
				{
					keyFrameIt = --keyFrames.end();
				}
			}

			return *keyFrameIt;
		}
		else if (playMode == Animation::PlayMode::LOOPED)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (++keyFrameIt == keyFrames.end())
					keyFrameIt = keyFrames.begin();
			}

			return *keyFrameIt;
		}
		else if (playMode == Animation::PlayMode::REVERSED)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (++keyFrameItReverse == keyFrames.rend())
				{
					--keyFrameItReverse;
					break;
				}
			}

			return *keyFrameItReverse;
		}
		else if (playMode == Animation::PlayMode::LOOP_REVERSED)
		{
			while (currentTime >= frameDuration)
			{
				currentTime -= frameDuration;
				if (++keyFrameItReverse == keyFrames.rend())
					keyFrameItReverse = keyFrames.rbegin();
			}

			return *keyFrameItReverse;
		}
		else
		{
			if (playMode == Animation::PlayMode::NORMAL || playMode == Animation::PlayMode::LOOPED)
				return *keyFrameIt;
			else if (playMode == Animation::PlayMode::REVERSED || playMode == Animation::PlayMode::LOOP_REVERSED)
				return *keyFrameItReverse;
			else
				return keyFrames[0];
		}
	}
	else
	{
		if (playMode == Animation::PlayMode::NORMAL || playMode == Animation::PlayMode::LOOPED)
			return *keyFrameIt;
		else if (playMode == Animation::PlayMode::REVERSED || playMode == Animation::PlayMode::LOOP_REVERSED)
			return *keyFrameItReverse;
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
	keyFrameIt = keyFrames.begin();
	keyFrameItReverse = keyFrames.rbegin();
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
	keyFrameIt = keyFrames.begin();
	keyFrameItReverse = keyFrames.rbegin();
	restartFrameTimer();
}
