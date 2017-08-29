#pragma once

#include <memory>
#include "EventManager.h"

class Actor;

class Component
{
	friend class Actor;

protected:
	const int id;
	EventManager* eventManager;
	Actor* owner;
public:
	Component(int id, EventManager* eventManager, Actor* owner) : id(id), eventManager(eventManager), owner(owner) {};
	virtual void update(float dt) {};
	virtual void draw() {};
	int getId() const { return id; };
};