#pragma once

#include "EventData.h"
#include "Utils.h"

struct EventLevelReload : public EventData
{
	static constexpr unsigned int eventId = getGUIDConst();
	EventLevelReload() : EventData(eventId) {};
};