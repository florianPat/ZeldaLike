#include "EventManager.h"

EventManager::EventManager() : eventListenerMap()
{
}

bool EventManager::addListener(int eventType, DelegateFunction & delegateFunction)
{
	auto eventListenerList = eventListenerMap.find(eventType);

	if (eventListenerList == eventListenerMap.end())
	{
		eventListenerMap.emplace(eventType, std::list<DelegateFunction>());
		eventListenerList = eventListenerMap.find(eventType);
	}

	for (auto it = eventListenerList->second.begin(); it != eventListenerList->second.end();
		++it)
	{
		if (delegateFunction.first == it->first)
		{
			std::cout << "Attempting to double - register a delegate" << std::endl;
			return false;
		}
	}

	eventListenerList->second.push_back(delegateFunction);
	return true;
}

bool EventManager::removeListener(int eventType, DelegateFunction & delegateFunction)
{
	bool success = false;
	auto findIt = eventListenerMap.find(eventType);
	if (findIt != eventListenerMap.end())
	{
		for (auto it = findIt->second.begin(); it != findIt->second.end(); ++it)
		{
			if (delegateFunction.first == it->first)
			{
				findIt->second.erase(it);
				success = true;
				break;
			}
		}
	}

	return success;
}

bool EventManager::TriggerEvent(std::unique_ptr<EventData> eventData)
{
	bool processed = false;
	auto findIt = eventListenerMap.find(eventData->getEventId());
	if (findIt != eventListenerMap.end())
	{
		for (auto it = findIt->second.begin(); it != findIt->second.end(); ++it)
		{
			it->second(eventData.get());
			processed = true;
		}
	}
	return processed;
}
