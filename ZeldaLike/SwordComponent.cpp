#include "SwordComponent.h"
#include "Utils.h"
#include "Actor.h"
#include "EventIsHitByPlayer.h"

const unsigned int SwordComponent::id = utils::getGUID();

SwordComponent::SwordComponent(TextureAtlas & textureAtlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner)
	: atlas(textureAtlas), physics(physics), renderTarget(renderTarget), currentFrame(), hitClock(), pos(nullptr), boundingBox(Physics::OBB()), 
	isHitting(nullptr), collisionIdsToAdd(physics.getAllCollisionIdsWhichContain("EnemyOrg")), Component(id, eventManager, owner)
{
	std::unique_ptr<Physics::Body> bodyUni = std::make_unique<Physics::Body>(sf::Vector2f{ 0.0f, 0.0f }, "PlayerSword", &boundingBox, &collisionId, true);
	body = physics.addElementPointer(std::move(bodyUni));

	currentFrame = atlas.findRegion("woodsword")->getRegion();
	currentFrame.setOrigin(currentFrame.getLocalBounds().width / 2.0f, currentFrame.getLocalBounds().height);
	//TOOD: Think about collider size!
	boundingBox.collider.obb.width = currentFrame.getGlobalBounds().width;
	boundingBox.collider.obb.height = currentFrame.getGlobalBounds().height;

	currentFrame.setScale(0.6f, 0.6f);

	auto playerComponent = owner->getComponent<PlayerComponent>(PlayerComponent::id);
	pos = playerComponent->getSwordPosRect();
	isHitting = playerComponent->getIsHitting();
	viewDir = playerComponent->getViewDir();
}

void SwordComponent::update(float dt)
{
	assert(pos != nullptr);
	currentFrame.setPosition((float)pos->left, (float)pos->top);
	
	if(*isHitting)
	{
		elapsedTime += hitClock.getElapsedTime();
		if (elapsedTime >= timeForHit)
		{
			boundingBox.collider.obb.setAngle(0.0f);
			*isHitting = false;
			collisionId.clear();
		}
		else
		{
			boundingBox.collider.obb.setAngle( utils::lerp(boundingBox.collider.obb.getAngle(), destAngle, elapsedTime / timeForHit) );
		}
	}

	if (body->getIsTriggerd())
	{
		eventManager->TriggerEvent(std::make_unique<EventIsHitByPlayer>(7.0f, body->getTriggerInformation().triggerElementCollision));
	}

	boundingBox.collider.obb.pos = currentFrame.getPosition();
}

void SwordComponent::draw()
{
	auto pos = currentFrame.getPosition();
	currentFrame.setRotation(boundingBox.collider.obb.getAngle());
	renderTarget.draw(currentFrame);
}

gomSort::SortKey SwordComponent::sort()
{
	float add = 0.0f;
	if (*viewDir != PlayerComponent::ViewDir::TOP) 
		add = currentFrame.getGlobalBounds().height / 2.0f;

	auto rotation = currentFrame.getRotation();
	currentFrame.setRotation(0.0f);
	float y = currentFrame.getGlobalBounds().top + add;
	currentFrame.setRotation(rotation);

	return gomSort::SortKey{ 0, y };
}

void SwordComponent::hit()
{
	hitClock.restart();
	elapsedTime = sf::Time::Zero;
	*isHitting = true;
	if (collisionId.empty())
		collisionId = collisionIdsToAdd;

	switch (*viewDir)
	{
		case PlayerComponent::ViewDir::BOTTOM:
		{
			boundingBox.collider.obb.setAngle(90.0f);
			destAngle = 270.0f;
			break;
		}

		case PlayerComponent::ViewDir::TOP:
		{
			boundingBox.collider.obb.setAngle(270.0f);
			destAngle = 450.0f;
			break;
		}

		case PlayerComponent::ViewDir::RIGHT:
		{
			boundingBox.collider.obb.setAngle(0.0f);
			destAngle = 180.0f;
			break;
		}

		case PlayerComponent::ViewDir::LEFT:
		{
			boundingBox.collider.obb.setAngle(180.0f);
			destAngle = 360.0f;
			break;
		}
		default:
		{
			InvalidCodePath;
			break;
		}
	}
}