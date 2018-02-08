#pragma once

#include "Component.h"
#include "Physics.h"
#include <map>
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "Animation.h"
#include "Utils.h"
#include "EventGetPlayerPos.h"
#include "EventIsHitByPlayer.h"
#include "GameObjectManager.h"

class OrgEnemyComponent : public Component
{
	static constexpr float speed = 20.0f;
	sf::RenderWindow& renderTarget;
	Physics& physics;
	TextureAtlas atlas;
	std::map<std::string, Animation> animations;
	sf::Sprite currentFrame;
	sf::Vector2f startingPos;
	Physics::Collider boundingBox;
	Physics::Body* body = nullptr;
	const sf::Vector2f* playerPos;
	sf::Vector2f force;
	Physics::Body* bodyPlayerHit = nullptr;
	float health = 100.0f;
	GameObjectManager& gom;
private:
	std::function<void(EventData*)> eventGetPlayerPosFunction = std::bind(&OrgEnemyComponent::eventGetPlayerPosHandler, this, std::placeholders::_1);
	DelegateFunction delegateGetPlayerPos = utils::getDelegateFromFunction(eventGetPlayerPosFunction);
	void eventGetPlayerPosHandler(EventData* eventData);
private:
	std::function<void(EventData*)> eventIsHitByPlayerFunction = std::bind(&OrgEnemyComponent::eventIsHitByPlayerHandler, this, std::placeholders::_1);
	DelegateFunction delegateIsHitByPlayer = utils::getDelegateFromFunction(eventIsHitByPlayerFunction);
	void eventIsHitByPlayerHandler(EventData* eventData);
public:
	static const unsigned int id;
public:
	OrgEnemyComponent(sf::Vector2f& startingPos, TextureAtlas& textureAtlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner, GameObjectManager& gom, int i);
	void update(float dt) override;
	void draw() override;
	gomSort::SortKey sort() override { return gomSort::SortKey{ 0, body->getPos().y }; }
};