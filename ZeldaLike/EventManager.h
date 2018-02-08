#pragma once

#include <unordered_map>
#include "EventData.h"
#include <iostream>
#include <functional>
#include <memory>

typedef std::pair<unsigned int, std::function<void(EventData*)>> DelegateFunction;

class EventManager
{
	friend class Level;

	std::unordered_map<unsigned int, std::list<DelegateFunction>> eventListenerMap;
	std::vector<std::pair<unsigned int, DelegateFunction>> eventDeleterMap;
public:
	EventManager();
	bool addListener(unsigned int eventType, DelegateFunction& delegateFunction);
	void removeListener(unsigned int eventType, DelegateFunction& delegateFunction);
	bool TriggerEvent(std::unique_ptr<EventData> eventData);
private:
	void removeListeners();
};