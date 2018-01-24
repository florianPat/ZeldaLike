#pragma once

#include "EventData.h"

struct EventLevelReload : public EventData
{
	//TODO: Automate this! 
	//Test the changes!
	static const unsigned int eventId = 0xd7885633;
	EventLevelReload() : EventData(eventId) {};
};