#include "TiledMap.h"
#include <fstream>
#include <stdlib.h>
#include "Assets.h"
#include "Utils.h"

TiledMap::TiledMap(const std::string & filepath) : tiles(), layers(), objectGroups(), 
												   textureSprite(), texture()
{
	std::ifstream file;
	file.open(filepath);

	if (!file)
		utilsLog("Cant open file!");
	if (!file.is_open())
		utilsLog("Cant open file!");

	std::string temp;
	std::getline(file, temp);

	if (!file.eof())
	{
		std::string lineContent;
		std::getline(file, lineContent);


		if (!utils::isWordInLine("orthogonal", lineContent))
			utilsLog("Map has to be orthogonal!");

		if (!utils::isWordInLine("right-down", lineContent))
			utilsLog("Maps render-order has to be right-down!");


		mapWidth = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
		mapHeight = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

		tileWidth = atoi(getLineContentBetween(lineContent, "tilewidth", '"').c_str());
		tileHeight = atoi(getLineContentBetween(lineContent, "tileheight", '"').c_str());

		lineContent = ParseTiles(file);

		ParseLayer(file, lineContent);

		ParseObjectGroups(file, lineContent);

		if (!utils::isWordInLine("</map>", lineContent))
		{
			utilsLog("We should be at the end of the file!");
		}

		MakeRenderTexture();
	}
}

std::vector<Physics::Collider> TiledMap::getObjectGroup(const std::string& objectGroupName)
{
	auto result = objectGroups.find(objectGroupName);
	if (result != objectGroups.end())
		return result->second.objects;
	else
	{
		std::vector<Physics::Collider> result;
		InvalidCodePath;
		return result;
	}
}

std::vector<TiledMap::ObjectGroup> TiledMap::getObjectGroups()
{
	std::vector<ObjectGroup> result;

	for (auto it = objectGroups.begin(); it != objectGroups.end(); ++it)
	{
		result.push_back(it->second);
	}

	return result;
}

void TiledMap::draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(textureSprite);
}

size_t TiledMap::getEndOfWord(const std::string & word, const std::string & lineContent, bool* result)
{
	size_t o = 0;
	*result = false;
	while (o < lineContent.size() && !(*result))
	{
		std::string searchWord(word);
		auto it = searchWord.begin();
		o = lineContent.find(it[0], o);
		++it;
		for (; o < lineContent.size(); ++it)
		{
			if (it != searchWord.end())
			{
				if (lineContent.at(++o) == it[0])
					continue;
				else
					break;
			}
			else
			{
				*result = true;
				break;
			}
		}
	}

	return ++o;
}

std::string TiledMap::getLineContentBetween(std::string & lineContent, const std::string & endOfFirst, char secound)
{
		std::string result;

		bool resultValue;
		size_t widthEndPos = getEndOfWord(endOfFirst, lineContent, &resultValue);
		if (resultValue)
		{
			lineContent.erase(0, widthEndPos += 2);

			size_t kommaPos = lineContent.find(secound);

			result = lineContent.substr(0, kommaPos);

			lineContent.erase(0, ++kommaPos);
		}

		return result;
}

void TiledMap::ParseLayer(std::ifstream & file, std::string& lineContent)
{
	while (utils::isWordInLine("<layer", lineContent))
	{
		std::string layerName = getLineContentBetween(lineContent, "name", '"');
		int layerWidth = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
		int layerHeight = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

		layers.emplace(layerName, Layer{ layerName, layerWidth, layerHeight, std::vector<Tile>() });

		auto currentLayer = layers.find(layerName);

		std::getline(file, lineContent); //  <data encoding="csv">
		if (!utils::isWordInLine("csv", lineContent))
			utilsLog("Maps encoding has to be \"csv\"");

		std::getline(file, lineContent); //Begin of encoding

		for (int y = 0; y < layerHeight; ++y)
		{
			for (int x = 0; x < layerWidth; ++x)
			{
				size_t kommaPos = lineContent.find(',');
				int nextTileId = atoi(lineContent.substr(0, kommaPos).c_str());
				lineContent.erase(0, ++kommaPos);

				Tile nextTile{ 0, 0, 0, nullptr };
				if (tiles.find(nextTileId) != tiles.end())
					nextTile = tiles.find(nextTileId)->second;
				currentLayer->second.tiles.push_back(nextTile);
			}
			std::getline(file, lineContent);
		}
		std::getline(file, lineContent); // </layer>
		std::getline(file, lineContent); //Maybe new <layer>
	}
}

void TiledMap::ParseObjectGroups(std::ifstream & file, std::string & lineContent)
{
	//ObjectGroup
	while (utils::isWordInLine("<objectgroup", lineContent))
	{
		std::string objectGroupName = getLineContentBetween(lineContent, "name", '"');
		std::getline(file, lineContent);

		std::vector<Physics::Collider> objectVector;
		while (!utils::isWordInLine("</objectgroup>", lineContent))
		{
			int x = atoi(getLineContentBetween(lineContent, "x", '"').c_str());
			int y = atoi(getLineContentBetween(lineContent, "y", '"').c_str());
			int width = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
			int height = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

			objectVector.push_back(sf::FloatRect((float)x, (float)y, (float)width, (float)height));

			std::getline(file, lineContent);
		}
		objectGroups.emplace(objectGroupName, ObjectGroup{ objectGroupName, objectVector });

		std::getline(file, lineContent);
	}
}

void TiledMap::MakeRenderTexture()
{
	if (texture.create(mapWidth*tileWidth, mapHeight*tileHeight))
	{
		texture.clear();

		for (auto it = layers.begin(); it != layers.end(); ++it)
		{
			for (int y = 0; y < mapHeight; ++y)
			{
				for (int x = 0; x < mapWidth; ++x)
				{
					Layer currentLayer = it->second;
					std::shared_ptr<sf::Texture> source = currentLayer.tiles[mapWidth * y + x].source;
					if (source == nullptr)
						continue;
					sf::Sprite sprite(*source);
					sprite.setPosition((float)x * tileWidth, (float)y * tileHeight);
					texture.draw(sprite);
				}
			}
		}
		texture.display();

		textureSprite = sf::Sprite(texture.getTexture());
	}
	else
		utilsLog("Could not create Render Texture");
}

std::string TiledMap::ParseTiles(std::ifstream & file)
{
	std::string lineContent;
	std::getline(file, lineContent);

	std::string temp;
	std::getline(file, temp); // <grid...

	while (utils::isWordInLine("<tileset", lineContent))
	{
		int firstgrid = atoi(getLineContentBetween(lineContent, "firstgid", '"').c_str());
		int tileCount = atoi(getLineContentBetween(lineContent, "tilecount", '"').c_str());
		for (int i = 0; i < tileCount; ++i)
		{
			std::getline(file, lineContent);
			int id = atoi(getLineContentBetween(lineContent, "id", '"').c_str()) + firstgrid;

			std::getline(file, lineContent);
			int width = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
			int height = atoi(getLineContentBetween(lineContent, "height", '"').c_str());
			std::string source = getLineContentBetween(lineContent, "source", '"');
			tiles.emplace(id, Tile{ id, width, height, Assets::textureAssetManager.getOrAddRes(source) });

			std::getline(file, lineContent); //</tile>
		}
		std::getline(file, lineContent); //</tileset>
		std::getline(file, lineContent); //Maybe new <tileset>...
	}

	return lineContent;
}
