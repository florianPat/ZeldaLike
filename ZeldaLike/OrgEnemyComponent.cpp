#include "OrgEnemyComponent.h"
#include "EventHitPlayer.h"
#include "Actor.h"

const unsigned int OrgEnemyComponent::id = utils::getGUID();

void OrgEnemyComponent::eventGetPlayerPosHandler(EventData * eventData)
{
	EventGetPlayerPos* eventDataP = static_cast<EventGetPlayerPos*>(eventData);
	playerPos = eventDataP->pos;
}

void OrgEnemyComponent::eventIsHitByPlayerHandler(EventData * eventData)
{
	auto eventdata = static_cast<EventIsHitByPlayer*>(eventData);

	if (eventdata->bodyId == body->getId())
	{
		if (body->vel.x == -speed)
			force.x = 100.0f;
		else if (body->vel.x == speed)
			force.x = -100.0f;

		if (body->vel.y == -speed)
			force.y = 100.0f;
		else if (body->vel.y == speed)
			force.y = -100.0f;

		health -= eventdata->health;
		if (health <= 0.0f)
		{
			physics.removeElementById(body->getId());
			physics.removeElementById(bodyPlayerHit->getId());
			eventManager->removeListener(EventGetPlayerPos::eventId, delegateGetPlayerPos);
			eventManager->removeListener(EventIsHitByPlayer::eventId, delegateIsHitByPlayer);
			gom.destroyActor(owner->getId());
		}
	}
}

OrgEnemyComponent::OrgEnemyComponent(sf::Vector2f & startingPos, TextureAtlas & textureAtlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner, GameObjectManager& gom, int i)
	: startingPos(startingPos), atlas(textureAtlas), physics(physics), renderTarget(renderTarget),
	  currentFrame(), boundingBox(sf::FloatRect()), playerPos(nullptr), force(), gom(gom), Component(id, eventManager, owner)
{
	std::unique_ptr<Physics::Body> bodyUni = std::make_unique<Physics::Body>(startingPos, std::string("EnemyOrg" + std::to_string(i)), &boundingBox, false, false, std::vector<std::string>{"Blocked", "PlayerSword"});
	body = physics.addElementPointer(std::move(bodyUni));

	bodyPlayerHit = physics.addElementPointer(std::make_unique<Physics::Body>(startingPos, std::string("EnemyOrgPlayerHit" + std::to_string(i)), &boundingBox, true, false, std::vector<std::string>{"Player"}));

	animations.emplace("frontIdel", Animation{ { "front1" }, atlas });
	animations.emplace("backIdel", Animation{ { "back1" }, atlas});
	animations.emplace("leftIdel", Animation{ { "left1" }, atlas});
	animations.emplace("rightIdel", Animation{ { "right1" }, atlas });
	animations.emplace("frontWalk", Animation{ { "front1", "front2", "front3" }, atlas });
	animations.emplace("backWalk", Animation{ { "back1", "back2", "back3" }, atlas });
	animations.emplace("leftWalk", Animation{ { "left1", "left2", "left3" }, atlas });

	eventManager->addListener(EventGetPlayerPos::eventId, delegateGetPlayerPos);
	eventManager->addListener(EventIsHitByPlayer::eventId, delegateIsHitByPlayer);
}

void OrgEnemyComponent::update(float dt)
{
	body->vel = { 0.0f, 0.0f };

	assert(playerPos != nullptr);

	if (playerPos->y - currentFrame.getPosition().y < -25.0f)
		body->vel.y = -speed;
	else if (playerPos->y - currentFrame.getPosition().y > 25.0f)
		body->vel.y = speed;
	if (playerPos->x - currentFrame.getPosition().x < -25.0f)
		body->vel.x = -speed;
	else if (playerPos->x - currentFrame.getPosition().x > 25.0f)
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

	if (force.x != 0.0f || force.y != 0.0f)
	{
		if (force.x > 0.0f)
			force.x -= 2.0f;
		else
			force.x += 2.0f;

		if (force.y > 0.0f)
			force.y -= 2.0f;
		else
			force.y += 2.0f;

		body->vel = force;
	}

	currentFrame.setPosition(body->getPos() + body->vel * dt);

	Physics::applySpriteToBoundingBox(currentFrame, boundingBox);

	if (bodyPlayerHit->getIsTriggerd())
		eventManager->TriggerEvent(std::make_unique<EventHitPlayer>(25.0f));
}

void OrgEnemyComponent::draw()
{
	currentFrame.setPosition(body->getPos());
	renderTarget.draw(currentFrame);
}
