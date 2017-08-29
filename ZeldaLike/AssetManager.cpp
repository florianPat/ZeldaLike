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
			utilsLog("Could not load texture!");
			return nullptr;
		}
		ressourceCache.insert({ filename, texture});
		return texture;
	}
}

bool TextureAssetManager::unloadNotUsedRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
	{
#if 0
		if (res->second.unique())
		{
			ressourceCache.erase(res);
			return true;
		}
		else
		{
			return false;
		}
#else
		//TOOD: Do I want that behaviour?
		res->second.reset();
		ressourceCache.erase(res);
		return true;
#endif
	}
	else
		return false;
}

void TextureAssetManager::clear()
{
	for (auto i = ressourceCache.begin(); i != ressourceCache.end(); )
	{
		if (i->second.unique())
		{
			i = ressourceCache.erase(i);
		}
		else
		{
			++i;
		}
	}
}

bool TextureAssetManager::isLoaded(const std::string & filename)
{
	auto i = ressourceCache.find(filename);
	return i != ressourceCache.end();
}