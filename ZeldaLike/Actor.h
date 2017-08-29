#pragma once

#include <unordered_map>
#include "Component.h"
#include <memory>

class Actor
{
	friend class GameObjectManager;

	const int id;
	std::unordered_map<int, std::shared_ptr<Component>> components;
private:
	void update(float dt);
	void draw();
public:
	Actor(const int& id);
	void addComponent(std::shared_ptr<Component> component);
	void removeComponent(int componentId);
	template <typename T> T* getComponent(int componentId); 
	int getId() const { return id; };
};

template<typename T>
inline T* Actor::getComponent(int componentId)
{
	auto result = components.find(componentId);
	if (result != components.end() && typeid(*result->second) == typeid(T))
	{
		return dynamic_cast<T*>(result->second.get());
	}
	else
		return nullptr;
}
