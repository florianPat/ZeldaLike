#pragma once

#include "Level.h"
#include <memory>

class Game
{
	sf::RenderWindow* window;
	std::unique_ptr<Level> currentLevel;
public:
	Game(sf::RenderWindow* window, std::string levelName);
};