#include "Level.h"
#include "EventLevelReload.h"
#include "PlayerComponent.h"
#include "SwordComponent.h"

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
	window.clear();

	map.draw(window);
	gom.sortActors();
	gom.drawActors();

	//physics.debugRenderBodies(window);

	window.display();
}

Level::Level(sf::RenderWindow & window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
clock(), gom(), eventManager(), map(tiledMapName, std::vector<std::string>{"OnGround"}, gom, eventManager, window)
{
	auto objectGroups = map.getObjectGroups();
	for (auto it = objectGroups.begin(); it != objectGroups.end(); ++it)
	{
		if (it->name == "Blocked")
		{
			physics.addElementValue(Physics::Body(std::string("Blocked"), it->objects));
		}
		else if (it->name == "PlayerStart")
		{
			Actor* playerP = gom.addActor();
			playerP->addComponent(std::make_unique<PlayerComponent>(sf::Vector2f(it->objects[0].collider.rect.left, it->objects[0].collider.rect.top), TextureAtlas("Player.atlas"), physics, window, &eventManager, playerP));
			playerP->addComponent(std::make_unique<SwordComponent>(TextureAtlas("Items.atlas"), physics, window, &eventManager, playerP));
		}
	}

	eventManager.addListener(EventLevelReload::eventId, delegateLevelReload);
}

std::string Level::Go()
{
	while (!endLevel && window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		updateModel();
		composeFrame();
	}

	return newLevel;
}
