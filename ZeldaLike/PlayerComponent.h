#pragma once

#include "Component.h"
#include "Physics.h"
#include <map>
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"

class PlayerComponent : public Component
{
	static constexpr float speed = 120.0f;
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, Animation> animations;
	sf::Sprite currentFrame;
	sf::Vector2f startingPos;
	Physics::Collider boundingBox;
	Physics::Body* body = nullptr;
	sf::View view;
public:
	PlayerComponent(sf::Vector2f& startingPos, TextureAtlas& textureAtlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
};