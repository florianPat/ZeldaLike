#pragma once

#include <unordered_map>
#include "EventData.h"
#include <iostream>
#include <functional>
#include <memory>

typedef std::pair<unsigned int, std::function<void(EventData*)>> DelegateFunction;

class EventManager
{
	std::unordered_map<unsigned int, std::list<DelegateFunction>> eventListenerMap;
public:
	EventManager();
	bool addListener(unsigned int eventType, DelegateFunction& delegateFunction);
	bool removeListener(unsigned int eventType, DelegateFunction& delegateFunction);
	bool TriggerEvent(std::unique_ptr<EventData> eventData);
};