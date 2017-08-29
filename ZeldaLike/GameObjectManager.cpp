#include "GameObjectManager.h"

GameObjectManager::GameObjectManager() : actors(), destroyActorQueue()
{
}

Actor* GameObjectManager::addActor(const int& id)
{
	auto result = actors.emplace(id, std::make_unique<Actor>(id));
	return result.first->second.get();
}

void GameObjectManager::destroyActor(int actorId)
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
	for (auto it = actors.begin(); it != actors.end(); ++it)
		it->second->draw();
}

Actor* GameObjectManager::getActor(int actorId)
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
