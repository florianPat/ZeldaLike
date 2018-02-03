#include "PlayerComponent.h"
#include "Utils.h"

PlayerComponent::PlayerComponent(sf::Vector2f & startingPos, TextureAtlas & textureAtlas, Physics & physics, sf::RenderWindow & renderTarget, EventManager * eventManager, Actor * owner)
	: startingPos(startingPos), atlas(textureAtlas), physics(physics), renderTarget(renderTarget), view(renderTarget.getDefaultView()),
	currentFrame(), boundingBox(sf::FloatRect()), Component(eventManager, owner)
{
	std::unique_ptr<Physics::Body> bodyUni = std::make_unique<Physics::Body>(startingPos, "Player", &boundingBox, false, false, std::vector<std::string>{"Blocked"});
	body = physics.addElementPointer(std::move(bodyUni));

	InkscapeAnimationElement iae("dataRaw/Player.svg");

	animations.emplace("frontIdel", InkscapeAnimation{ { "front1" }, atlas, iae });
	animations.emplace("backIdel", InkscapeAnimation{ { "back1" }, atlas, iae });
	animations.emplace("leftIdel", InkscapeAnimation{ { "left1" }, atlas, iae });
	animations.emplace("rightIdel", InkscapeAnimation{ { "right1" }, atlas, iae });
	animations.emplace("frontWalk", InkscapeAnimation{ { "front1", "front2", "front3" }, atlas, iae });
	animations.emplace("backWalk", InkscapeAnimation{ { "back1", "back2", "back3" }, atlas, iae });
	animations.emplace("leftWalk", InkscapeAnimation{ { "left1", "left2", "left3" }, atlas, iae });
	animations.emplace("rightWalk", InkscapeAnimation{ { "right1", "right2", "right3" }, atlas, iae });

	swordTest.setFillColor(sf::Color::Black);
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
	{
		auto anim = animations.find("rightWalk")->second;

		currentFrame = anim.getKeyFrame();
		auto rect = anim.getInkscapeAnimationElementKeyFrame("swordHand");
		swordTest.setPosition({ (float)rect.left + body->getPos().x, (float)rect.top + body->getPos().y });
		swordTest.setSize({ (float)rect.width, (float)rect.height });
	}
	else if (body->vel.x < 0)
	{
		auto anim = animations.find("leftWalk")->second;

		currentFrame = anim.getKeyFrame();
		auto rect = anim.getInkscapeAnimationElementKeyFrame("swordHand");
		swordTest.setPosition({ (float)rect.left + body->getPos().x, (float)rect.top + body->getPos().y });
		swordTest.setSize({ (float)rect.width, (float)rect.height });
	}
	else if (body->vel.y > 0)
	{
		auto anim = animations.find("frontWalk")->second;

		currentFrame = anim.getKeyFrame();
		auto rect = anim.getInkscapeAnimationElementKeyFrame("swordHand");
		swordTest.setPosition({ (float)rect.left + body->getPos().x, (float)rect.top + body->getPos().y });
		swordTest.setSize({ (float)rect.width, (float)rect.height });
	}
	else if (body->vel.y < 0)
	{
		currentFrame = animations.find("backWalk")->second.getKeyFrame();
	}
	else
	{
		auto anim = animations.find("frontIdel")->second;
		
		currentFrame = anim.getKeyFrame();
		auto rect = anim.getInkscapeAnimationElementKeyFrame("swordHand");
		swordTest.setPosition({ (float)rect.left + body->getPos().x, (float)rect.top + body->getPos().y });
		swordTest.setSize({ (float)rect.width, (float)rect.height });
	}

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
	
	renderTarget.draw(swordTest);

	view.setCenter(body->getPos());
	renderTarget.setView(view);
}
