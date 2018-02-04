#pragma once

#include "Component.h"
#include "SFML\Graphics.hpp"
#include "Utils.h"

class TiledMapRenderComponent : public Component
{
	const sf::Sprite sprite;
	sf::RenderWindow& renderTarget;
public:
	TiledMapRenderComponent(sf::Sprite& sprite, sf::RenderWindow& renderTarget, EventManager& em, Actor* owner) : sprite(sprite), renderTarget(renderTarget), Component(utils::getGUID(), &em, owner) {};
	void update(float dt) override {};
	void draw() override { renderTarget.draw(sprite); };
	gomSort::SortKey sort() override { return gomSort::SortKey{ 0, sprite.getPosition().y }; }
};