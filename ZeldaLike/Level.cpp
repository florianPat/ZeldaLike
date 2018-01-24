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

	physics.update(dt);
}

void Level::composeFrame()
{
	window->clear();

	map.draw(*window);
	gom.sortActors();
	gom.drawActors();

	physics.debugRenderBodies(*window);

	window->display();
}

Level::Level(sf::RenderWindow * window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
map(tiledMapName), clock(), gom(), eventManager()
{
	auto objectGroups = map.getObjectGroups();
	for (auto it = objectGroups.begin(); it != objectGroups.end(); ++it)
	{
		if (it->name == "Blocked")
		{
			physics.addElementValue(Physics::Body(std::string("Blocked"), it->objects));
		}
	}

	eventManager.addListener(EventLevelReload::eventId, delegateLevelReload);
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
