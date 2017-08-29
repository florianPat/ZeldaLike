#pragma once

#include <memory>
#include <unordered_map>
#include "SFML\Graphics.hpp"

class TextureAssetManager
{
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> ressourceCache;
public:
	std::shared_ptr<sf::Texture> getOrAddRes(const std::string& filename);
	bool unloadNotUsedRes(const std::string& filename);
	void clear();
	bool isLoaded(const std::string& filename);
};