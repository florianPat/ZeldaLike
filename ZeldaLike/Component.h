#pragma once

#include <memory>
#include "EventManager.h"
#include "gomSort.h"
#include "Utils.h"

class Actor;

class Component
{
	friend class Actor;

protected:
	const unsigned int id;
	EventManager* eventManager;
	Actor* owner;
public:
	Component(EventManager* eventManager, Actor* owner) : id(getGUID()), eventManager(eventManager), owner(owner) {};
	virtual void update(float dt) {};
	virtual void draw() {};
	virtual gomSort::SortKey sort() { return gomSort::SortKey{ 0, 0.0f }; };
	unsigned int getId() const { return id; };
};