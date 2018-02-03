#pragma once

#include <unordered_map>
#include <map>
#include <queue>
#include "Actor.h"

class GameObjectManager
{
	std::unordered_map<unsigned int, std::unique_ptr<Actor>> actors;
	std::multimap<gomSort::SortKey, unsigned long long, gomSort::SortCompare> sortedActors; //TODO: Think about not making this a long long?
	std::vector<int> destroyActorQueue;
public:
	GameObjectManager();
	Actor* addActor();
	void destroyActor(unsigned int actorId);
	void updateActors(float dt);
	void drawActors();
	void sortActors();
	Actor* getActor(unsigned int actorId);
private:
	void destroyActors();
	unsigned int getActorId(unsigned long long id);
	unsigned int getComponentId(unsigned long long id);
};