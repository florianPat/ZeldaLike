#pragma once

class EventData
{
protected:
	const int eventId;
public:
	EventData(int eventId) : eventId(eventId) {};
	int getEventId() const { return eventId; };
};