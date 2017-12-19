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

	body1->vel.x = 0.0f;
	body1->vel.y = 0.0f;

	//body1->vel.x = -50.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		body1->vel.x = 50.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		body1->vel.x = -50.0f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		body1->vel.y = -50.0f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		body1->vel.y = 50.0f;
	}

	collider1.collider.circle.center = body1->getPos() + body1->vel * dt;

	physics.update(dt);
}

void Level::composeFrame()
{
	window->clear();

	map.draw(*window);
	gom.sortActors();
	gom.drawActors();

	collider1.collider.obb.pos = body1->getPos();

	physics.debugRenderBodies(*window);

	window->display();
}

Level::Level(sf::RenderWindow * window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
map(tiledMapName), clock(), gom(), eventManager(), collider1(Physics::Collider(Physics::FloatCircle(150.0f, 30.0f, 20.0f))),
body1(std::make_shared<Physics::Body>(sf::Vector2f(150.0f, 30.0f), std::string("one"), &collider1, false, false, std::vector<std::string>{ "two" }))
{
	auto objects = map.getObjectGroups();

	eventManager.addListener(EventLevelReload::EVENT_LEVEL_RELOAD_ID, delegateLevelReload);

	physics.addElementPointer(body1);
	physics.addElementPointer(body2);
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
