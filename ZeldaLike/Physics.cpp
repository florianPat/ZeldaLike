#include "Physics.h"
#include <assert.h>
#include <math.h>
#include <limits>
#include <iostream>

void Physics::handleCollision(std::shared_ptr<Body>& itBody, std::shared_ptr<Body>& collideElementBody, Collider & bodyCollider,
	const Collider& elementCollider)
{
	if (itBody->isTrigger || collideElementBody->isTrigger)
	{
		if (bodyCollider.intersects(elementCollider))
		{
			if (itBody->isTrigger)
			{
				itBody->triggered = true;
				itBody->triggerInformation.triggerElementCollision = collideElementBody->physicsElement.id;
				return;
			}
			else if (collideElementBody->isTrigger)
			{
				itBody->triggered = true;
				itBody->triggerInformation.triggerElementCollision = collideElementBody->physicsElement.id;
				return;
			}
		}
	}

	sf::Vector2f minTransVec = {};
	if (bodyCollider.collide(elementCollider, &minTransVec))
	{
		if (minTransVec.x > 0.0f)
		{
			itBody->vel.x = 0;
			itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::LEFT;
		}
		else if (minTransVec.x < 0.0f)
		{
			itBody->vel.x = 0;
			itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::RIGHT;
		}
		if (minTransVec.y > 0.0f)
		{
			itBody->vel.y = 0;
			itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::HEAD;
		}
		else if (minTransVec.y < 0.0f)
		{
			itBody->vel.y = 0;
			itBody->triggerInformation.triggerBodyPart = Body::TriggerBodyPart::SHOES;
		}

		itBody->pos += minTransVec;
	}
}

void Physics::Collider::getPointsAxis(sf::Vector2f * points, sf::Vector2f * axis) const
{
	if (type == Collider::Type::obb)
	{
		OBB bodyOBB = collider.obb;

		points[0] = { bodyOBB.pos };
		points[1] = { bodyOBB.pos.x + bodyOBB.width, bodyOBB.pos.y };
		points[2] = { bodyOBB.pos.x + bodyOBB.width, bodyOBB.pos.y + bodyOBB.height };
		points[3] = { bodyOBB.pos.x, bodyOBB.pos.y + bodyOBB.height };

		//Global origin
		sf::Vector2f origin = bodyOBB.pos + bodyOBB.origin;

		for (int i = 0; i < 4; ++i)
		{
			points[i] = sf::Vector2f(bodyOBB.pos + (points[i].x - origin.x) * bodyOBB.xAxis + (points[i].y - origin.y) * bodyOBB.yAxis);
		}

		axis[0] = bodyOBB.xAxis;
		axis[1] = bodyOBB.yAxis;
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
	}
}

sf::Vector2f Physics::Collider::getProjectionMinMax(const sf::Vector2f * points, const sf::Vector2f & axis) const
{
	sf::Vector2f result = { points[0].x * axis.x + points[0].y * axis.y, points[0].x * axis.x + points[0].y * axis.y };

	for (int i = 1; i < 4; ++i)
	{
		float proj = points[i].x * axis.x + points[i].y * axis.y;

		if (proj < result.x)
			result.x = proj;
		else if (proj > result.y)
			result.y = proj;
	}

	return result;
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

				it->second->pos += it->second->vel /** dt*/;
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

			sf::RectangleShape body;

			switch (collider->type)
			{
				case Collider::Type::rect:
				{
					sf::FloatRect colliderRect = collider->collider.rect;

					body.setSize(sf::Vector2f(colliderRect.width, colliderRect.height));
					body.setPosition(sf::Vector2f(colliderRect.left, colliderRect.top));
					body.setFillColor(sf::Color::Yellow);
					
					window.draw(body);

					break;
				}
				case Collider::Type::obb:
				{
					OBB collideOBB = collider->collider.obb;

					body.setPosition(collideOBB.pos);
					body.setSize(sf::Vector2f{ collideOBB.width, collideOBB.height });
					body.setOrigin(collideOBB.origin);
					body.setRotation(collideOBB.angle*180/collideOBB.PI);
					body.setFillColor(sf::Color::Yellow);
#if 0
					sf::Vector2f points[4] = { { collideOBB.pos },{ collideOBB.pos.x + collideOBB.width, collideOBB.pos.y },
											 { collideOBB.pos.x + collideOBB.width, collideOBB.pos.y + collideOBB.height },
											 { collideOBB.pos.x, collideOBB.pos.y + collideOBB.height } };

					//Global origin
					sf::Vector2f origin = collideOBB.pos + collideOBB.origin;

					for (int i = 0; i < 4; ++i)
					{
						points[i] = sf::Vector2f(collideOBB.pos + (points[i].x - origin.x) * collideOBB.xAxis + (points[i].y - origin.y) * collideOBB.yAxis);
					}
					
					for (unsigned int i = 0; i < body.getPointCount(); ++i)
					{
						sf::Vector2f myPoint = points[i];
						sf::Vector2f point = body.getPoint(i);
						point = body.getTransform().transformPoint(point);
						sf::Transform transform = body.getTransform();

						std::cout << myPoint.x << " " << myPoint.y << "---" << point.x << " " << point.y << std::endl;
					}
#endif

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

Physics::Collider::Type Physics::Collider::GetType() const
{
	return type;
}

bool Physics::Collider::intersects(const Collider & other) const
{
	if (other.type == Type::rect && type == Type::rect)
	{
		return collider.rect.intersects(other.collider.rect);
	}
	else
	{
		sf::Vector2f axis[4] = {};

		sf::Vector2f s1Points[4] = {};
		sf::Vector2f s2Points[4] = {};

		getPointsAxis(s1Points, axis);
		other.getPointsAxis(s2Points, axis + 2);

		for (int i = 0; i < 8; ++i)
		{
			sf::Vector2f s1MinMax = getProjectionMinMax(s1Points, axis[i]);
			sf::Vector2f s2MinMax = getProjectionMinMax(s2Points, axis[i]);

			if ((s2MinMax.x > s1MinMax.y || s2MinMax.y < s1MinMax.x))
				return false;
			else
			{
				continue;
			}
		}

		return true;
	}
}

bool Physics::Collider::collide(const Collider & other, sf::Vector2f *minTransVec) const
{
	if (other.type == Type::rect && type == Type::rect)
	{
		bool result = collider.rect.intersects(other.collider.rect);
		
		if (result)
		{
			sf::FloatRect rect = collider.rect;
			sf::FloatRect otherRect = other.collider.rect;
			
			*minTransVec = { rect.left - (otherRect.left + otherRect.width), 0 };
			std::vector<sf::Vector2f> corners;
			corners.emplace_back(sf::Vector2f{ (rect.left + rect.width) - otherRect.left, 0 });
			corners.emplace_back(sf::Vector2f{ 0, rect.top - (otherRect.top + otherRect.height) });
			corners.emplace_back(sf::Vector2f{ 0, (rect.top + rect.height) - otherRect.top });

			for (auto it = corners.begin(); it != corners.end(); ++it)
			{
				if (std::sqrtf(minTransVec->x * minTransVec->x + minTransVec->y * minTransVec->y) > std::sqrtf(it->x * it->x + it->y * it->y))
				{
					*minTransVec = *it;
				}
			}
		}

		minTransVec->x *= -1;
		minTransVec->y *= -1;

		return result;
	}
	else
	{
		sf::Vector2f axis[4] = {};

		sf::Vector2f s1Points[4] = {};
		sf::Vector2f s2Points[4] = {};

		float angle = 0.0f;

		float o = std::numeric_limits<float>::max();
		sf::Vector2f minAxis = { 0.0f, 0.0f };

		getPointsAxis(s1Points, axis);
		other.getPointsAxis(s2Points, axis + 2);

		for (int i = 0; i < 4; ++i)
		{
			sf::Vector2f s1MinMax = getProjectionMinMax(s1Points, axis[i]);
			sf::Vector2f s2MinMax = getProjectionMinMax(s2Points, axis[i]);

			if ((s2MinMax.x > s1MinMax.y || s2MinMax.y < s1MinMax.x))
				return false;
			else
			{
				float overlap = s1MinMax.y > s2MinMax.y ? -(s2MinMax.y - s1MinMax.x) : (s1MinMax.y - s2MinMax.x);
				if (std::fabsf(overlap) < std::fabsf(o))
				{
					o = overlap;
					minAxis = axis[i];
					if (i < 2)
					{
						if (type == Type::obb)
							angle = collider.obb.angle;
						else
							angle = 0.0f;
					}
					else
					{
						if (other.type == Type::obb)
							angle = other.collider.obb.angle;
						else
							angle = 0.0f;
					}
				}
			}
		}

		*minTransVec = -sf::Vector2f(o * minAxis.x, o * minAxis.y);

		return true;
	}
}

//NOTE: angle from degrees in radians, because cosf uses radians, but in matrix of SFML in Shape it uses degrees, so you have to convert back and forth...
//TODO: Do I have to think about going over 360 degree here?
Physics::OBB::OBB(float left, float top, float width, float height, float angle) : angle(angle*PI/180), pos(sf::Vector2f{ left, top }), width(width), height(height),
																				   xAxis(cosf(this->angle), sinf(this->angle)), yAxis((-sinf(this->angle)), cosf(this->angle)),
																				   origin(0.0f, 0.0f)
{
}

Physics::OBB::OBB(sf::Vector2f & topLeft, float width, float height, float angle) : angle(angle*PI/180), pos(topLeft), width(width), height(height),
																					xAxis(cosf(this->angle), sinf(this->angle)), yAxis((-sinf(this->angle)), cosf(this->angle)),
																					origin(0.0f, 0.0f)
{
}

Physics::OBB::OBB(float left, float top, float width, float height, float angle, sf::Vector2f origin) : angle(angle*PI / 180), pos(sf::Vector2f{ left, top }), width(width), height(height),
																										xAxis(cosf(this->angle), sinf(this->angle)), yAxis((-sinf(this->angle)), cosf(this->angle)),
																										origin(origin)
{
}

Physics::OBB::OBB(sf::Vector2f & topLeft, float width, float height, float angle, sf::Vector2f origin) : angle(angle*PI / 180), pos(topLeft), width(width), height(height),
																										 xAxis(cosf(this->angle), sinf(this->angle)), yAxis((-sinf(this->angle)), cosf(this->angle)),
																										 origin(origin)
{
}

void Physics::OBB::setAngle(float newAngle)
{
	angle = newAngle*PI/180;
	xAxis = sf::Vector2f(cosf(angle), sinf(angle));
	yAxis = sf::Vector2f(-sinf(angle), cosf(angle));
}

float Physics::OBB::getAngle() const
{
	return angle;
}
