#pragma once

#include "Component.h"
#include "Physics.h"
#include <map>
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "InkscapeAnimation.h"
#include "Utils.h"

class PlayerComponent : public Component
{
	static constexpr float speed = 120.0f;
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, InkscapeAnimation> animations;
	sf::Sprite currentFrame;
	sf::Vector2f startingPos;
	Physics::Collider boundingBox;
	Physics::Body* body = nullptr;
	sf::View view;
	sf::IntRect swordPosRect;
public:
	static constexpr int id = getGUIDConst();
public:
	PlayerComponent(sf::Vector2f& startingPos, TextureAtlas& textureAtlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	const sf::IntRect* getSwordPosRect() const;
	gomSort::SortKey sort() override { return gomSort::SortKey{ 0, body->getPos().y }; }
};