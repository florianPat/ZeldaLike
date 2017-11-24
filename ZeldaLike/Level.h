#pragma once

#include <SFML/Graphics.hpp>
#include "TiledMap.h"
#include "Assets.h"
#include "Physics.h"
#include <iostream>
#include "GameObjectManager.h"
#include "EventManager.h"

class Level
{
private:
	sf::RenderWindow* window;
	Physics physics;
	TiledMap map;
	sf::Clock clock;
	EventManager eventManager;
	GameObjectManager gom;

	std::string levelName;

	std::string newLevel = "";
	bool endLevel = false;
private:
	std::function<void(EventData*)> eventLevelReloadFunction = std::bind(&Level::eventLevelReloadHandler, this, std::placeholders::_1);
	static constexpr int delegateLevelReloadId = 0;
	DelegateFunction delegateLevelReload = std::make_pair(delegateLevelReloadId, eventLevelReloadFunction);
private:
	void eventLevelReloadHandler(EventData* eventData);
	void eventWinHandler(EventData* eventData);
private:
	Physics::Collider collider1 = Physics::Collider(sf::FloatRect(0.0f, 30.0f, 20.0f, 20.0f));
	std::shared_ptr<Physics::Body> body1 = std::make_shared<Physics::Body>(sf::Vector2f(0.0f, 30.0f), std::string("one"), &collider1, false, false, std::vector<std::string>{ "two" } );
	Physics::Collider collider2 = Physics::Collider(sf::FloatRect(100.0f, 30.0f, 20.0f, 20.0f));
	std::shared_ptr<Physics::Body> body2 = std::make_shared<Physics::Body>(sf::Vector2f(100.0f, 30.0f), std::string("two"), &collider2, false, false);
private:
	virtual void updateModel();
	virtual void composeFrame();
public:
	Level(sf::RenderWindow* window, std::string tiledMapName);
	std::string Go();
};