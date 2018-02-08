#pragma once

#include "PlayerComponent.h"

class SwordComponent : public Component
{
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	sf::Sprite currentFrame;
	Physics::Collider boundingBox;
	Physics::Body* body = nullptr;
	const sf::IntRect* pos;
	sf::Clock hitClock;
	sf::Time timeForHit = sf::seconds(2.0f);
	sf::Time elapsedTime = sf::Time();
	bool* isHitting;
	float destAngle = 0.0f;
	std::vector<std::string> collisionId;
	std::vector<std::string> collisionIdsToAdd;
	const PlayerComponent::ViewDir* viewDir;
public:
	static const unsigned int id;
public:
	SwordComponent(TextureAtlas& textureAtlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	gomSort::SortKey sort() override;
	void hit();
};