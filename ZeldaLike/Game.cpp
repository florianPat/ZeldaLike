#include "Game.h"

Game::Game(sf::RenderWindow& window, std::string levelName) : window(window),
currentLevel(std::make_unique<Level>(window, levelName))
{
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		std::string newLevel = currentLevel->Go();

		if (newLevel == "")
		{
			window.close();
			return;
		}

		currentLevel = std::make_unique<Level>(window, newLevel);
	}
}
