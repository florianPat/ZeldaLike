#pragma once

#include "EventData.h"
#include "Utils.h"

struct EventIsHitByPlayer : public EventData
{
	static const unsigned int eventId;
	float health;
	std::string bodyId;
	EventIsHitByPlayer(float health, const std::string& bodyId);
};