#pragma once

#include "PlayerComponent.h"

class SwordComponent : public Component
{
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, Animation> animations;
	sf::Sprite currentFrame;
	Physics::Collider boundingBox;
	Physics::Body* body = nullptr;
	const sf::IntRect* pos;
public:
	static constexpr int id = getGUIDConst();
public:
	SwordComponent(TextureAtlas& textureAtlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	gomSort::SortKey sort() override { return gomSort::SortKey{ 0, currentFrame.getGlobalBounds().top + currentFrame.getGlobalBounds().height / 2.0f }; }
};