#pragma once

#include "Component.h"
#include "Physics.h"
#include <map>
#include "TextureAtlas.h"
#include "SFML\Graphics.hpp"
#include "Animation.h"
#include "Utils.h"
#include "EventGetPlayerPos.h"

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
private:
	std::function<void(EventData*)> eventGetPlayerPosFunction = std::bind(&OrgEnemyComponent::eventGetPlayerPosHandler, this, std::placeholders::_1);
	DelegateFunction delegateGetPlayerPos = utils::getDelegateFromFunction(eventGetPlayerPosFunction);
private:
	void eventGetPlayerPosHandler(EventData* eventData);
public:
	static constexpr unsigned int id = getGUIDConst();
public:
	OrgEnemyComponent(sf::Vector2f& startingPos, TextureAtlas& textureAtlas, Physics& physics, sf::RenderWindow& renderTarget, EventManager* eventManager, Actor* owner);
	void update(float dt) override;
	void draw() override;
	gomSort::SortKey sort() override { return gomSort::SortKey{ 0, body->getPos().y }; }
};