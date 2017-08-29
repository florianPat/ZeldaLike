#include "Game.h"
#include "Level.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 600), "gppcc9-Game");
	window.setFramerateLimit(60);

	Game game(&window, "");

	return 0;
}