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

	body1->vel.x = 50.0f;
	collider1.collider.rect.left = body1->getPos().x + body1->vel.x * dt;

	physics.update(dt);
}

void Level::composeFrame()
{
	window->clear();

	map.draw(*window);
	gom.drawActors();

	collider1.collider.obb.origin = body1->getPos();

	physics.debugRenderBodies(*window);

	window->display();
}

Level::Level(sf::RenderWindow * window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
map(tiledMapName), clock(), gom(), eventManager()
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
