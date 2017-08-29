#include "Physics.h"
#include <assert.h>

void Physics::handleCollision(std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& it,
	std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& collideElement,
	sf::FloatRect & bodyRect, const sf::FloatRect & elementRect)
{
	if (bodyRect.intersects(elementRect))
	{
		if (it->second->isTrigger)
		{
			it->second->triggered = true;
			it->second->triggerInformation.triggerElementCollision = collideElement->second->physicsElement.id;
			return; //NOTE: Safty first...
		}
		else if (collideElement->second->isTrigger)
		{
			it->second->triggered = true;
			it->second->triggerInformation.triggerElementCollision = collideElement->second->physicsElement.id;
		}
		
		if (it->second->vel.x >= 0 && it->second->physicsElement.rightBody.intersects(elementRect))
		{
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::RIGHT;
				return;
			}
			it->second->vel.x = 0;
			it->second->pos.x = elementRect.left - bodyRect.width;
		}
		else if (it->second->vel.x < 0 && it->second->physicsElement.leftBody.intersects(elementRect))
		{
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::LEFT;
				return;
			}
			it->second->vel.x = 0;
			it->second->pos.x = elementRect.left + elementRect.width;
		}
		else if (it->second->vel.y >= 0 && it->second->physicsElement.shoes.intersects(elementRect))
		{
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::SHOES;
				return;
			}
			it->second->pos.y = elementRect.top - bodyRect.height + 0.25f;
			it->second->vel.y = 0.0f;
		}
		else if (it->second->vel.y < 0 && it->second->physicsElement.head.intersects(elementRect))
		{
			if (it->second->triggered)
			{
				it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::HEAD;
				return;
			}
			it->second->pos.y = elementRect.top + elementRect.height - 0.25f;
			it->second->vel.y = 0.0f;
		}
	}
}

Physics::Physics() : bodies()
{
}

void Physics::update(float dt)
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it)
	{
		if (!it->second->isStatic || it->second->isTrigger)
		{
			it->second->triggered = false;
			it->second->triggerInformation.triggerElementCollision = "";
			it->second->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::NONE;

			if (!it->second->isTrigger)
			{
				it->second->updateShoes();
				it->second->updateLeftRightBody();
				it->second->updateHead();
			}

			for (auto collisionIdIt = it->second->physicsElement.collisionIds->begin(); collisionIdIt != it->second->physicsElement.collisionIds->end(); ++collisionIdIt)
			{
				auto collideElement = bodies.find(*collisionIdIt);
				if (collideElement != bodies.end())
				{
					if (it->second->physicsElement.collidersInPointer)
					{
						sf::FloatRect& bodyRect = *it->second->physicsElement.colliders.collidersPointer;

						if (collideElement->second->physicsElement.collidersInPointer)
						{
							sf::FloatRect elementRect = *collideElement->second->physicsElement.colliders.collidersPointer;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
						else
						{
							sf::FloatRect elementRect = collideElement->second->physicsElement.colliders.collidersValue;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
					}
					else
					{
						sf::FloatRect& bodyRect = it->second->physicsElement.colliders.collidersValue;

						if (collideElement->second->physicsElement.collidersInPointer)
						{
							sf::FloatRect elementRect = *collideElement->second->physicsElement.colliders.collidersPointer;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
						else
						{
							sf::FloatRect elementRect = collideElement->second->physicsElement.colliders.collidersValue;
							
							handleCollision(it, collideElement, bodyRect, elementRect);
						}
					}
				}

				it->second->pos += it->second->vel * dt;
			}
		}
	}
}

void Physics::debugRenderBodies(sf::RenderWindow & window)
{
	for (auto it = bodies.begin(); it != bodies.end(); ++it)
	{
		if (!it->second->isStatic)
		{
			sf::RectangleShape head, shoes, body;
			head.setSize(sf::Vector2f(it->second->physicsElement.head.width, it->second->physicsElement.head.height));
			head.setPosition(sf::Vector2f(it->second->physicsElement.head.left, it->second->physicsElement.head.top));
			head.setFillColor(sf::Color::Red);
			window.draw(head);

			shoes.setSize(sf::Vector2f(it->second->physicsElement.shoes.width, it->second->physicsElement.shoes.height));
			shoes.setPosition(sf::Vector2f(it->second->physicsElement.shoes.left, it->second->physicsElement.shoes.top));
			shoes.setFillColor(sf::Color::Blue);
			window.draw(shoes);

			body.setSize(sf::Vector2f(it->second->physicsElement.leftBody.width, it->second->physicsElement.leftBody.height));
			body.setPosition(sf::Vector2f(it->second->physicsElement.leftBody.left, it->second->physicsElement.leftBody.top));
			body.setFillColor(sf::Color::Yellow);
			window.draw(body);
		}
	}
}

void Physics::addElementPointer(std::shared_ptr<Body> body)
{
	bodies.emplace(body->physicsElement.id, body);
}

void Physics::addElementValue(Body body)
{
	std::shared_ptr<Body> bodyPtr = std::make_shared<Body>(Body(body.physicsElement.id, body.physicsElement.colliders.collidersValue, false, true, {}));
	bodies.emplace(body.physicsElement.id, bodyPtr);
}

bool Physics::removeElementById(std::string & id)
{
	return bodies.erase(id);
}

void Physics::Body::updateShoes()
{
	if (!isStatic)
	{
		physicsElement.shoes.left = physicsElement.colliders.collidersPointer->left + 5.0f;
		physicsElement.shoes.top = physicsElement.colliders.collidersPointer->top + physicsElement.colliders.collidersPointer->height - 15.0f;
		physicsElement.shoes.width = physicsElement.colliders.collidersPointer->width - 10.0f;
		physicsElement.shoes.height = 15.0f;
	}
}

void Physics::Body::updateLeftRightBody()
{
	if (!isStatic)
	{
		physicsElement.leftBody.left = physicsElement.colliders.collidersPointer->left;
		physicsElement.leftBody.top = physicsElement.colliders.collidersPointer->top + 15.0f;
		physicsElement.leftBody.width = physicsElement.colliders.collidersPointer->width / 2.0f;
		physicsElement.leftBody.height = physicsElement.colliders.collidersPointer->height - 30.0f;

		physicsElement.rightBody.left = physicsElement.colliders.collidersPointer->left + physicsElement.colliders.collidersPointer->width / 2.0f;
		physicsElement.rightBody.top = physicsElement.colliders.collidersPointer->top + 15.0f;
		physicsElement.rightBody.width = physicsElement.colliders.collidersPointer->width / 2.0f;
		physicsElement.rightBody.height = physicsElement.colliders.collidersPointer->height - 30.0f;
	}
}

void Physics::Body::updateHead()
{
	if (!isStatic)
	{
		physicsElement.head.left = physicsElement.colliders.collidersPointer->left + 5.0f;
		physicsElement.head.top = physicsElement.colliders.collidersPointer->top;
		physicsElement.head.width = physicsElement.colliders.collidersPointer->width - 10.0f;
		physicsElement.head.height = 15.0f;
	}
}

Physics::Body::Body(sf::Vector2f& pos, std::string name, sf::FloatRect* collider, std::vector<std::string>* collisionId, bool isTrigger, bool isStatic)
	: isStatic(isStatic), isTrigger(isTrigger), pos(pos), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIdPointer = nullptr;
	this->physicsElement.collisionIds = collisionId;
	this->physicsElement.collidersInPointer = true;
	this->physicsElement.colliders.collidersPointer = collider;
}

Physics::Body::Body(sf::Vector2f & pos, std::string name, sf::FloatRect * collider, bool isTrigger, bool isStatic, std::vector<std::string> collisionId)
	: isStatic(isStatic), isTrigger(isTrigger), pos(pos), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIdPointer = std::make_shared<std::vector<std::string>>(collisionId);
	this->physicsElement.collisionIds = std::move(physicsElement.collisionIdPointer).get();
	this->physicsElement.collidersInPointer = true;
	this->physicsElement.colliders.collidersPointer = collider;
}

Physics::Body::Body(std::string name, sf::FloatRect collider, bool isTrigger, bool isStatic, std::vector<std::string> collisionId)
	: isStatic(isStatic), isTrigger(isTrigger), pos(0.0f, 0.0f), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIdPointer = std::make_shared<std::vector<std::string>>(collisionId);
	this->physicsElement.collisionIds = std::move(physicsElement.collisionIdPointer).get();
	this->physicsElement.collidersInPointer = false;
	this->physicsElement.colliders.collidersValue = collider;
}

bool Physics::Body::getIsTriggerd()
{
	return triggered;
}

sf::Vector2f& Physics::Body::getPos()
{
	assert(!isStatic); //For now, see TODO below

	if (!isStatic)
		return pos;
	else
		return pos; //TODO: Throw exeption...
}

void Physics::Body::setPos(sf::Vector2f newPos)
{
	assert(!isStatic); //For now, see TODO below

	if (!isStatic)
		pos = newPos;
	//TODO: else Throw exeption;
}

Physics::Body::TriggerInformation & Physics::Body::getTriggerInformation()
{
	return triggerInformation;
}

std::string & Physics::Body::getId()
{
	return physicsElement.id;
}
