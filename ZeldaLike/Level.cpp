#include "Level.h"
#include "EventLevelReload.h"

void Level::eventLevelReloadHandler(EventData* eventData)
{
	newLevel = levelName;
	endLevel = true;
}

void Level::eventWinHandler(EventData * eventData)
{
	std::cout << "WIN" << std::endl;
}

void Level::updateModel()
{
	float dt = clock.restart().asSeconds();

	gom.updateActors(dt);

	body->vel = { 0.0f, 0.0f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		body->vel.y += 80.0f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		body->vel.x += 80.0f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		body->vel.y -= 80.0f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		body->vel.x -= 80.0f;

	body->setPos(body->getPos() + body->vel * dt);

	physics.update(dt);
}

void Level::composeFrame()
{
	window->clear();

	map.draw(*window);
	gom.sortActors();
	gom.drawActors();

	collider.collider.rect.left = body->getPos().x;
	collider.collider.rect.top = body->getPos().y;

	physics.debugRenderBodies(*window);

	window->display();
}

Level::Level(sf::RenderWindow * window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
map(tiledMapName), clock(), gom(), eventManager(), collider(sf::FloatRect(10, 10, 50, 50)), body(std::make_shared<Physics::Body>(sf::Vector2f{ 0.0f, 0.0f }, std::string("Player"), &collider, false, false, std::vector<std::string>{"Blocked"}))
{
	auto objectGroups = map.getObjectGroups();
	for (auto it = objectGroups.begin(); it != objectGroups.end(); ++it)
	{
		if (it->name == "Blocked")
		{
			physics.addElementValue(Physics::Body(std::string("Blocked"), it->objects));
		}
	}

	physics.addElementPointer(body);

	eventManager.addListener(EventLevelReload::EVENT_LEVEL_RELOAD_ID, delegateLevelReload);
}

std::string Level::Go()
{
	while (!endLevel && window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		updateModel();
		composeFrame();
	}

	return newLevel;
}
