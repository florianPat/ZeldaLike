#pragma once

#include "EventData.h"
#include "Utils.h"
#include "SFML\Graphics.hpp"

struct EventGetPlayerPos : public EventData
{
	static constexpr unsigned int eventId = getGUIDConst();
	const sf::Vector2f* pos = nullptr;
	EventGetPlayerPos(const sf::Vector2f* pos) : pos(pos), EventData(eventId) {};
};