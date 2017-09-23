#include "Physics.h"
#include <assert.h>
#include <math.h>

void Physics::handleCollision(std::shared_ptr<Body>& itBody, std::shared_ptr<Body>& collideElementBody, Collider & bodyCollider, 
							  const Collider& elementCollider)
{
	if (bodyCollider.intersects(elementCollider))
	{
		if (itBody->isTrigger)
		{
			itBody->triggered = true;
			itBody->triggerInformation.triggerElementCollision = collideElementBody->physicsElement.id;
			return; //NOTE: Safty first...
		}
		else if (collideElementBody->isTrigger)
		{
			itBody->triggered = true;
			itBody->triggerInformation.triggerElementCollision = collideElementBody->physicsElement.id;
		}
	
		if (itBody->vel.x >= 0 && itBody->physicsElement.rightBody.intersects(elementCollider))
		{
			if (itBody->triggered)
			{
				itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::RIGHT;
				return;
			}
			itBody->vel.x = 0;
			//TODO: Add Minimum translation vector (#http://wiki.roblox.com/index.php?title=2D_Collision_Detection)
			//itBody->pos.x = elementRect.left - bodyRect.width;
		}
		else if (itBody->vel.x < 0 && itBody->physicsElement.leftBody.intersects(elementCollider))
		{
			if (itBody->triggered)
			{
				itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::LEFT;
				return;
			}
			itBody->vel.x = 0;
			//itBody->pos.x = elementRect.left + elementRect.width;
		}
		else if (itBody->vel.y >= 0 && itBody->physicsElement.shoes.intersects(elementCollider))
		{
			if (itBody->triggered)
			{
				itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::SHOES;
				return;
			}
			itBody->vel.y = 0.0f;
			//itBody->pos.y = elementRect.top - bodyRect.height + 0.25f;
		}
		else if (itBody->vel.y < 0 && itBody->physicsElement.head.intersects(elementCollider))
		{
			if (itBody->triggered)
			{
				itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::HEAD;
				return;
			}
			itBody->vel.y = 0.0f;
			//itBody->pos.y = elementRect.top + elementRect.height - 0.25f;
		}
	}
}

void Physics::Collider::getPointsAxis(sf::Vector2f * points, sf::Vector2f * axis) const
{
	if (type == Collider::Type::obb)
	{
		OBB bodyOBB = collider.obb;

		points[0] = bodyOBB.origin;
		points[1] = bodyOBB.origin + bodyOBB.width * bodyOBB.xAxis;
		points[2] = bodyOBB.origin + bodyOBB.width * bodyOBB.xAxis + bodyOBB.height * bodyOBB.yAxis;
		points[3] = bodyOBB.origin + bodyOBB.height * bodyOBB.yAxis;

		axis[0] = bodyOBB.xAxis;
		axis[1] = bodyOBB.yAxis;
		axis[2] = -bodyOBB.xAxis;
		axis[3] = -bodyOBB.yAxis;
	}
	else
	{
		sf::FloatRect bodyRect = collider.rect;

		points[0] = { bodyRect.left, bodyRect.top };
		points[1] = { bodyRect.left + bodyRect.width, bodyRect.top };
		points[2] = { bodyRect.left + bodyRect.width, bodyRect.top + bodyRect.height };
		points[3] = { bodyRect.left, bodyRect.top + bodyRect.height };

		axis[0] = { 1.0f, 0.0f };
		axis[1] = { 0.0f, 1.0f };
		axis[2] = -axis[0];
		axis[3] = -axis[1];
	}
}

void Physics::Collider::getProjection(float * proj, const sf::Vector2f * points, const sf::Vector2f * axis1, const sf::Vector2f * axis2) const
{
	int projI = 0;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			proj[projI++] = points[i].x * axis1[j].x + points[i].y * axis1[j].y;
		}
		for (int j = 0; j < 4; ++j)
		{
			proj[projI++] = points[i].x * axis2[j].x + points[i].y * axis2[j].y;
		}
	}
}

sf::Vector2f Physics::Collider::getMinMax(const float * proj) const
{
	float min = proj[0], max = proj[0];

	for (int i = 1; i < 8; ++i)
	{
		if (min > proj[i])
			min = proj[i];

		if (max < proj[i])
			max = proj[i];
	}

	return sf::Vector2f{ min, max };
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
				it->second->physicsElement.updateBodyParts();
			}

			for (auto collisionIdIt = it->second->physicsElement.collisionIds->begin(); collisionIdIt != it->second->physicsElement.collisionIds->end(); ++collisionIdIt)
			{
				auto collideElementIt = bodies.find(*collisionIdIt);
				if (collideElementIt != bodies.end())
				{
					auto collideElementBody = collideElementIt->second;
					auto itBody = it->second;

					Collider& bodyRect = *itBody->physicsElement.getCollider();
					Collider& elementRect = *collideElementBody->physicsElement.getCollider();

					handleCollision(itBody, collideElementBody, bodyRect, elementRect);
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
			Collider* collider = it->second->physicsElement.getCollider();

			switch (collider->type)
			{
				case Collider::Type::rect:
				{
					sf::FloatRect colliderRect = collider->collider.rect;

					sf::RectangleShape body;

					body.setSize(sf::Vector2f(colliderRect.width, colliderRect.height));
					body.setPosition(sf::Vector2f(colliderRect.left, colliderRect.top));
					body.setFillColor(sf::Color::Yellow);
					window.draw(body);

					break;
				}
			}
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

Physics::Body::Body(sf::Vector2f& pos, std::string name, Collider* collider, std::vector<std::string>* collisionId, bool isTrigger, bool isStatic)
	: isStatic(isStatic), isTrigger(isTrigger), pos(pos), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIds = collisionId;
	this->physicsElement.collidersInPointer = true;
	this->physicsElement.colliders.collidersPointer = collider;
}

Physics::Body::Body(sf::Vector2f & pos, std::string name, Collider * collider, bool isTrigger, bool isStatic, std::vector<std::string> collisionId)
	: isStatic(isStatic), isTrigger(isTrigger), pos(pos), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIdPointer = collisionId;
	this->physicsElement.collisionIds = &physicsElement.collisionIdPointer;
	this->physicsElement.collidersInPointer = true;
	this->physicsElement.colliders.collidersPointer = collider;
}

Physics::Body::Body(std::string name, Collider collider, bool isTrigger, bool isStatic, std::vector<std::string> collisionId)
	: isStatic(isStatic), isTrigger(isTrigger), pos(0.0f, 0.0f), physicsElement{}
{
	this->physicsElement.id = name;
	this->physicsElement.collisionIdPointer = collisionId;
	this->physicsElement.collisionIds = &physicsElement.collisionIdPointer;
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

Physics::Collider * Physics::PhysicElement::getCollider() const
{
	if (collidersInPointer)
		return colliders.collidersPointer;
	else
		return (Collider *) &colliders.collidersValue;
}

void Physics::PhysicElement::initBodyParts()
{
	if (getCollider()->type == Collider::Type::rect)
	{
		head = Collider(sf::FloatRect());
		shoes = Collider(sf::FloatRect());
		leftBody = Collider(sf::FloatRect());
		rightBody = Collider(sf::FloatRect());
	}
	else
	{
		head = Collider(OBB());
		shoes = Collider(OBB());
		leftBody = Collider(OBB());
		rightBody = Collider(OBB());
	}
}

//TODO: Implement!
void Physics::PhysicElement::updateBodyParts()
{
}

Physics::Collider::Collider() : type(Type::rect), collider{ {} }
{
}

Physics::Collider::Collider(sf::FloatRect & rect) : type(Type::rect), collider{ {rect.left, rect.top, rect.width, rect.height} }
{
}

Physics::Collider::Collider(OBB & obb) : type(Type::obb), collider{ {} }
{
	collider.obb = obb;
}

bool Physics::Collider::intersects(const Collider & other) const
{
	if (other.type == Type::rect && type == Type::rect)
	{
		return collider.rect.intersects(other.collider.rect);
	}
	else
	{
		sf::Vector2f s1Points[4] = {};
		sf::Vector2f s1Axis[4] = {};

		sf::Vector2f s2Points[4] = {};
		sf::Vector2f s2Axis[4] = {};

		getPointsAxis(s1Points, s1Axis);
		other.getPointsAxis(s2Points, s2Axis);

		float s1Proj[8] = {};
		getProjection(s1Proj, s1Points, s1Axis, s2Axis);
		sf::Vector2f s1MinMax = getMinMax(s1Proj);

		float s2Proj[8] = {};
		getProjection(s2Proj, s2Points, s1Axis, s2Axis);
		sf::Vector2f s2MinMax = getMinMax(s2Proj);

		if (s2MinMax.x > s1MinMax.y || s2MinMax.y < s1MinMax.x)
			return true;
		else
			return false;
	}
}

Physics::OBB::OBB(float left, float top, float width, float height, float angle) : angle(angle), origin(sf::Vector2f{ left, top }), width(width), height(height),
																				   xAxis(cosf(angle), sinf(angle)), yAxis((-sinf(angle)), cosf(angle))
{
}

Physics::OBB::OBB(sf::Vector2f & topLeft, float width, float height, float angle) : angle(angle), origin(topLeft), width(width), height(height),
																					xAxis(cosf(angle), sinf(angle)), yAxis((-sinf(angle)), cosf(angle))
{
}