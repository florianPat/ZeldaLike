#include "SwordComponent.h"

#include "Actor.h"

SwordComponent::SwordComponent(TextureAtlas & textureAtlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner)
	: atlas(textureAtlas), physics(physics), renderTarget(renderTarget), currentFrame(), pos(nullptr), boundingBox(sf::FloatRect()), Component(id, eventManager, owner)
{
	std::unique_ptr<Physics::Body> bodyUni = std::make_unique<Physics::Body>(sf::Vector2f{ 0.0f, 0.0f }, "PlayerSword", &boundingBox, true);
	body = physics.addElementPointer(std::move(bodyUni));

	currentFrame = atlas.findRegion("woodsword")->getRegion();
	currentFrame.setOrigin(currentFrame.getLocalBounds().width / 2.0f, currentFrame.getLocalBounds().height);
	currentFrame.setScale(0.6f, 0.6f);

	auto playerComponent = owner->getComponent<PlayerComponent>(PlayerComponent::id);
	pos = playerComponent->getSwordPosRect();
}

void SwordComponent::update(float dt)
{
	assert(pos != nullptr);
	currentFrame.setPosition((float)pos->left, (float)pos->top);

	boundingBox.collider.rect.left = currentFrame.getPosition().x;
	boundingBox.collider.rect.top = currentFrame.getPosition().y;
	boundingBox.collider.rect.width = (float)currentFrame.getTextureRect().width;
	boundingBox.collider.rect.height = (float)currentFrame.getTextureRect().height;
}

void SwordComponent::draw()
{
	auto pos = currentFrame.getPosition();
	if(pos.x != 0 || pos.y != 0)
		renderTarget.draw(currentFrame);
}
