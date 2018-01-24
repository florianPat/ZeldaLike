#include "GameObjectManager.h"
#include <iostream>
#include "Utils.h"

GameObjectManager::GameObjectManager() : actors(), destroyActorQueue(), sortedActors()
{
}

Actor* GameObjectManager::addActor()
{
	unsigned int id = utils::getGUID();

	auto result = actors.emplace(id, std::make_unique<Actor>(id));
	return result.first->second.get();
}

void GameObjectManager::destroyActor(unsigned int actorId)
{
	destroyActorQueue.push_back(actorId);
}

void GameObjectManager::updateActors(float dt)
{
	for (auto it = actors.begin(); it != actors.end(); ++it)
		it->second->update(dt);

	destroyActors();
}

void GameObjectManager::drawActors()
{
	for (auto it = sortedActors.begin(); it != sortedActors.end(); ++it)
	{
		int actorId = getActorId(it->second);
		int componentId = getComponentId(it->second);
		auto actor = actors.find(actorId);
		if (actor != actors.end())
			actor->second->draw(componentId);
		else
		{
			InvalidCodePath;
		}
	}
}

void GameObjectManager::sortActors()
{
	for (auto it = actors.begin(); it != actors.end(); ++it)
	{
		it->second->sort(sortedActors);
	}
}

Actor* GameObjectManager::getActor(unsigned int actorId)
{
	auto it = actors.find(actorId);
	if (it != actors.end())
		return it->second.get();
	else
		return nullptr;
}

void GameObjectManager::destroyActors()
{
	if (!destroyActorQueue.empty())
	{
		for (auto it = destroyActorQueue.begin(); it != destroyActorQueue.end(); ++it)
		{
			auto actorIt = actors.find(*it);
			if (actorIt != actors.end())
			{
				for (auto componentIt = actorIt->second->components.begin(); componentIt != actorIt->second->components.end(); componentIt = actorIt->second->components.begin())
				{
					actorIt->second->removeComponent(componentIt->second->getId());
				}
				actors.erase(actorIt);
			}
		}
		destroyActorQueue.clear();
	}
}

unsigned int GameObjectManager::getActorId(unsigned long long id)
{
	return (id >> 32);
}

unsigned int GameObjectManager::getComponentId(unsigned long long id)
{
	return (id & 0xffffffff);
}
