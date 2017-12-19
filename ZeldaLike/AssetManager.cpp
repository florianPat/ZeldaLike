#include "AssetManager.h"
#include "Utils.h"

std::shared_ptr<sf::Texture> TextureAssetManager::getOrAddRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
		return res->second;
	else
	{
		std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
		if(!texture->loadFromFile(filename))
		{
			utilsLogBreak("Could not load texture!");
			return nullptr;
		}

		currentSize += texture->getSize().x * texture->getSize().y * sizeof(int); //One pixel is one int also in SFML, right?
		if (currentSize > maxSize)
		{
			do
			{
				//TODO: Check if emplace does do its job at the back...
				auto it = ressourceCache.begin();
				currentSize -= it->second->getSize().x * it->second->getSize().y * sizeof(int);
				ressourceCache.erase(it);
			} while (currentSize > maxSize);
		}

		ressourceCache.emplace(std::pair<std::string, std::shared_ptr<sf::Texture>>{ filename, texture});
		return texture;
	}
}

bool TextureAssetManager::unloadNotUsedRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
	{
		res->second.reset();
		ressourceCache.erase(res);
		return true;
	}
	else
		return false;
}

void TextureAssetManager::clear()
{
	ressourceCache.clear();
}

bool TextureAssetManager::isLoaded(const std::string & filename)
{
	auto i = ressourceCache.find(filename);
	return i != ressourceCache.end();
}