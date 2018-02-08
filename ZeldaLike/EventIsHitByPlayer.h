#pragma once

#include "EventData.h"
#include "Utils.h"

struct EventIsHitByPlayer : public EventData
{
	static const unsigned int eventId;
	float health;
	EventIsHitByPlayer(float health);
};