#include "PlayerComponent.h"
#include "Utils.h"

PlayerComponent::PlayerComponent(sf::Vector2f & startingPos, TextureAtlas & textureAtlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner)
	: startingPos(startingPos), atlas(textureAtlas), physics(physics), renderTarget(renderTarget), view(renderTarget.getDefaultView()),
	currentFrame(), boundingBox(sf::FloatRect()), Component(eventManager, owner)
{
	std::unique_ptr<Physics::Body> bodyUni = std::make_unique<Physics::Body>(startingPos, "Player", &boundingBox, false, false, std::vector<std::string>{"Blocked"});
	body = physics.addElementPointer(std::move(bodyUni));

	animations.emplace("frontIdel", Animation{ { "front1" }, atlas });
	animations.emplace("backIdel", Animation{ { "back1" }, atlas});
	animations.emplace("leftIdel", Animation{ { "left1" }, atlas });
	animations.emplace("rightIdel", Animation{ { "right1" }, atlas });
	animations.emplace("frontWalk", Animation{ { "front1", "front2", "front3" }, atlas });
	animations.emplace("backWalk", Animation{ { "back1", "back2", "back3" }, atlas });
	animations.emplace("leftWalk", Animation{ { "left1", "left2", "left3" }, atlas });
	animations.emplace("rightWalk", Animation{ { "right1", "right2", "right3" }, atlas });
}

void PlayerComponent::update(float dt)
{
	body->vel = { 0.0f, 0.0f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		body->vel.y = -speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		body->vel.y = speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		body->vel.x = -speed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		body->vel.x = speed;

	if (body->vel.x > 0)
		currentFrame = animations.find("rightWalk")->second.getKeyFrame();
	else if (body->vel.x < 0)
		currentFrame = animations.find("leftWalk")->second.getKeyFrame();
	else if (body->vel.y > 0)
		currentFrame = animations.find("frontWalk")->second.getKeyFrame();
	else if (body->vel.y < 0)
		currentFrame = animations.find("backWalk")->second.getKeyFrame();
	else
		currentFrame = animations.find("frontIdel")->second.getKeyFrame();

	currentFrame.setPosition(body->getPos() + body->vel * dt);

	boundingBox.collider.rect.left = currentFrame.getPosition().x;
	boundingBox.collider.rect.top = currentFrame.getPosition().y;
	boundingBox.collider.rect.width = (float)currentFrame.getTextureRect().width;
	boundingBox.collider.rect.height = (float)currentFrame.getTextureRect().height;
}

void PlayerComponent::draw()
{
	currentFrame.setPosition(body->getPos());
	renderTarget.draw(currentFrame);

	view.setCenter(body->getPos());
	renderTarget.setView(view);
}
