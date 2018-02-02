#pragma once

#include <vector>
#include <string>
#include <SFML\Graphics.hpp>
#include <memory>
#include <unordered_map>

class TextureRegion
{
	friend class TextureAtlas;

	std::string textureAtlasFileName;
	std::string filename;
	sf::Vector2i xy;
	sf::Vector2i size;

	sf::Texture* atlasTexture;
	sf::Sprite regionSprite;
private:
	TextureRegion() = default;
	void initSprite();
public:
	std::string getAtlasFileName() { return textureAtlasFileName; }
	std::string getRegionName() { return filename; }
	sf::Vector2i getXY() { return xy; }
	sf::Vector2i getSize() { return size; }

	void setRegion(int x, int y, int widht, int height);
	sf::Sprite getRegion();
};

class TextureAtlas
{
	struct FileHeader
	{
		std::string name;
		sf::Vector2i size;
		std::string format;
		std::string filter[2];
		std::string repeat;
	};
public:
	TextureAtlas(const std::string& filepath);

	std::unique_ptr<TextureRegion> findRegion(const std::string& name) const;
	std::vector<TextureRegion> getRegions();
	void addRegion(const TextureRegion& adder);
private:
	std::string getLineContentBetweeen(std::string& lineContent, char first, char secound);
	sf::Vector2i getLineContentRegionValues(std::string& lineContent, char firstRealChar);
private:
	std::unordered_map<std::string, TextureRegion> textureAtlas;
	static constexpr int FILE_HEADER_LINE_SIZE = 5;
	static constexpr int FILE_LINES_PER_REGION = 7;
	FileHeader fileHeader;
};