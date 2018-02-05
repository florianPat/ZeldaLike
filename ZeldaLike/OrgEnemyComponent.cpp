#include "OrgEnemyComponent.h"

void OrgEnemyComponent::eventGetPlayerPosHandler(EventData * eventData)
{
	EventGetPlayerPos* eventDataP = static_cast<EventGetPlayerPos*>(eventData);
	playerPos = eventDataP->pos;
}

OrgEnemyComponent::OrgEnemyComponent(sf::Vector2f & startingPos, TextureAtlas & textureAtlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner)
	: startingPos(startingPos), atlas(textureAtlas), physics(physics), renderTarget(renderTarget),
	  currentFrame(), boundingBox(sf::FloatRect()), playerPos(nullptr), Component(id, eventManager, owner)
{
	std::unique_ptr<Physics::Body> bodyUni = std::make_unique<Physics::Body>(startingPos, "EnemyOrg", &boundingBox, false, false, std::vector<std::string>{"Blocked", "PlayerSword"});
	body = physics.addElementPointer(std::move(bodyUni));

	animations.emplace("frontIdel", Animation{ { "front1" }, atlas });
	animations.emplace("backIdel", Animation{ { "back1" }, atlas});
	animations.emplace("leftIdel", Animation{ { "left1" }, atlas});
	animations.emplace("rightIdel", Animation{ { "right1" }, atlas });
	animations.emplace("frontWalk", Animation{ { "front1", "front2", "front3" }, atlas });
	animations.emplace("backWalk", Animation{ { "back1", "back2", "back3" }, atlas });
	animations.emplace("leftWalk", Animation{ { "left1", "left2", "left3" }, atlas });

	//TODO: Find out why it works this way, but not with EventGetPlayerPos::eventId
	EventGetPlayerPos egpp(playerPos);
	eventManager->addListener(egpp.getEventId(), delegateGetPlayerPos);
}

void OrgEnemyComponent::update(float dt)
{
	body->vel = { 0.0f, 0.0f };

	assert(playerPos != nullptr);

	if (playerPos->y - currentFrame.getPosition().y < 0.0f)
		body->vel.y = -speed;
	if (playerPos->y - currentFrame.getPosition().y > 0.0f)
		body->vel.y = speed;
	if (playerPos->x - currentFrame.getPosition().x < 0.0f)
		body->vel.x = -speed;
	if (playerPos->x - currentFrame.getPosition().x > 0.0f)
		body->vel.x = speed;

	if (body->vel.x > 0)
	{
		currentFrame = animations.find("leftWalk")->second.getKeyFrame();
		currentFrame.setScale(-1.0f, 1.0f);
	}
	else if (body->vel.x < 0)
	{
		currentFrame = animations.find("leftWalk")->second.getKeyFrame();
		currentFrame.setScale(1.0f, 1.0f);
	}
	else if (body->vel.y > 0)
	{
		currentFrame = animations.find("frontWalk")->second.getKeyFrame();
		currentFrame.setScale(1.0f, 1.0f);
	}
	else if (body->vel.y < 0)
	{
		currentFrame = animations.find("backWalk")->second.getKeyFrame();
		currentFrame.setScale(1.0f, 1.0f);
	}
	else
	{
		currentFrame = animations.find("frontIdel")->second.getKeyFrame();
		currentFrame.setScale(1.0f, 1.0f);
	}

	currentFrame.setPosition(body->getPos() + body->vel * dt);

	Physics::applySpriteToBoundingBox(currentFrame, boundingBox);
}

void OrgEnemyComponent::draw()
{
	currentFrame.setPosition(body->getPos());
	renderTarget.draw(currentFrame);
}
