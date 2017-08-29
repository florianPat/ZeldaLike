#pragma once

#include "EventData.h"

struct EventLevelReload : public EventData
{
	static constexpr int EVENT_LEVEL_RELOAD_ID = 0xbf88343e;
	EventLevelReload() : EventData(EVENT_LEVEL_RELOAD_ID) {};
};