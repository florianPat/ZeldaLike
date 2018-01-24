#include "AssetManager.h"
#include "Utils.h"

sf::Texture* TextureAssetManager::getOrAddRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
		return res->second.get();
	else
	{
		std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
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
				//TODO: Emplace not does its job at the back... What should I do now? ;)
				auto it = ressourceCache.begin();
				currentSize -= it->second->getSize().x * it->second->getSize().y * sizeof(int);
				ressourceCache.erase(it);
			} while (currentSize > maxSize);
		}

		auto result = ressourceCache.emplace(std::pair<std::string, std::unique_ptr<sf::Texture>>{ filename, std::move(texture) });
		assert(result.second);
		return result.first->second.get();
	}
}

bool TextureAssetManager::unloadNotUsedRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
	{
		//NOTE: I think I do not have to do that
		//res->second.reset();
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