#pragma once

#include "EventData.h"

struct EventHitPlayer : public EventData
{
	static const unsigned int eventId;
	const float health;
	EventHitPlayer(float health);
};