#pragma once

#include <unordered_map>
#include "SFML\Graphics.hpp"
#include <memory>
#include "Physics.h"
#include "GameObjectManager.h"

class TiledMap
{
	struct Tile
	{
		int id;
		int width, height;
		sf::Texture* source;
	};
	struct Layer
	{
		std::string name;
		int width, height;
		std::vector<Tile> tiles;
	};
	struct ObjectGroup
	{
		std::string name;
		std::vector<Physics::Collider> objects;
	};
	std::unordered_map<int, Tile> tiles;
	std::unordered_map<std::string, Layer> layers;
	std::unordered_map<std::string, ObjectGroup> objectGroups;
	int mapWidth = 0, mapHeight = 0, tileWidth = 0, tileHeight = 0;

	sf::RenderTexture texture;
	sf::Sprite textureSprite;
public:
	TiledMap(const std::string& filepath, std::vector<std::string>& toGameObjects = std::vector<std::string>{}, GameObjectManager& gom = GameObjectManager{}, EventManager& em = EventManager(), sf::RenderWindow& window = sf::RenderWindow());
	std::vector<Physics::Collider> getObjectGroup(const std::string& objectGroupName);
	std::vector<ObjectGroup> getObjectGroups();
	void draw(sf::RenderWindow& renderWindow);
private:
	size_t getEndOfWord(const std::string& word, const std::string& lineContent, bool* result);
	std::string getLineContentBetween(std::string& lineContent, const std::string& endOfFirst, char secound);

	std::string ParseTiles(std::ifstream& file);
	void ParseLayer(std::ifstream& file, std::string& lineContent);
	void ParseObjectGroups(std::ifstream& file, std::string& lineContent);
	void MakeRenderTexture(std::vector<std::string>& toGameObjects, GameObjectManager& gom, EventManager& em, sf::RenderWindow& window);
};