#pragma once

#include <unordered_map>
#include "EventData.h"
#include <iostream>
#include <functional>
#include <memory>

typedef std::pair<int, std::function<void(EventData*)>> DelegateFunction;

class EventManager
{
	std::unordered_map<int, std::list<DelegateFunction>> eventListenerMap;
public:
	EventManager();
	bool addListener(int eventType, DelegateFunction& delegateFunction);
	bool removeListener(int eventType, DelegateFunction& delegateFunction);
	bool TriggerEvent(std::unique_ptr<EventData> eventData);
};