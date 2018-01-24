#pragma once

#include "EventData.h"
#include "Utils.h"

struct EventLevelReload : public EventData
{
	static constexpr unsigned int eventId = getGUID();
	EventLevelReload() : EventData(eventId) {};
};