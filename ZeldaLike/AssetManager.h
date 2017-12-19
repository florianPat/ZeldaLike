#pragma once

#include <memory>
#include <unordered_map>
#include "SFML\Graphics.hpp"
#include "Utils.h"

class TextureAssetManager
{
	static constexpr size_t maxSize = Gigabyte(4);
	size_t currentSize = 0;
	std::unordered_map<std::string, std::shared_ptr<sf::Texture>> ressourceCache;
public:
	std::shared_ptr<sf::Texture> getOrAddRes(const std::string& filename);
	bool unloadNotUsedRes(const std::string& filename);
	void clear();
	bool isLoaded(const std::string& filename);
};