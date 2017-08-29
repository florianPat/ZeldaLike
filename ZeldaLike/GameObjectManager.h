#pragma once

#include <unordered_map>
#include <queue>
#include "Actor.h"

class GameObjectManager
{
	std::unordered_map<int, std::unique_ptr<Actor>> actors;
	std::vector<int> destroyActorQueue;

public:
	GameObjectManager();
	Actor* addActor(const int& id);
	void destroyActor(int actorId);
	void updateActors(float dt);
	void drawActors();
	Actor* getActor(int actorId);
private:
	void destroyActors();
};