#pragma once

#include "EventData.h"
#include "Utils.h"
#include "SFML\Graphics.hpp"

struct EventGetPlayerPos : public EventData
{
	static const unsigned int eventId;
	const sf::Vector2f* pos = nullptr;
	EventGetPlayerPos(const sf::Vector2f* pos);
};