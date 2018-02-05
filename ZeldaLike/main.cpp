#include "Game.h"
#include "Level.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "Zelda-Like");
	window.setFramerateLimit(60);

	Game game(window, "testLevel.tmx");

	return 0;
}