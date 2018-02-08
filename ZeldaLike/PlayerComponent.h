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
public:
	enum class ViewDir
	{
		LEFT, RIGHT, TOP, BOTTOM
	};
private:
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
	bool isHitting = false;
	ViewDir viewDir = ViewDir::BOTTOM;
	float health = 100.0f;
	bool isDead = false;
	sf::Clock deadClock;
public:
	static const unsigned int id;
private:
	std::function<void(EventData*)> eventHitPlayerFunction = std::bind(&PlayerComponent::eventHitPlayerHandler, this, std::placeholders::_1);
	DelegateFunction delegateHitPlayer = utils::getDelegateFromFunction(eventHitPlayerFunction);
	void eventHitPlayerHandler(EventData* eventData);
public:
	PlayerComponent(sf::Vector2f& startingPos, TextureAtlas& textureAtlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	const sf::IntRect* getSwordPosRect() const;
	gomSort::SortKey sort() override { if (!isDead) return gomSort::SortKey{ 0, body->getPos().y }; else return gomSort::SortKey{255, 0.0f }; }
	bool* getIsHitting() const;
	const ViewDir* getViewDir() const;
};