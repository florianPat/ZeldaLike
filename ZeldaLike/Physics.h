#pragma once

#include "SFML\Graphics.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

//TODO: Add PhysicsElementGroup

class Physics
{
public:
	class FloatCircle
	{
	private:
		friend class Physics;
	public:
		float radius;
		sf::Vector2f center;
	public:
		FloatCircle(const sf::Vector2f& center, float radius);
		FloatCircle(float centerX, float centerY, float radius);
	private:
		FloatCircle() = default;
	};

	class OBB
	{
	private:
		friend class Physics;

		static constexpr float PI = 3.1415927f;

		float angle;
		sf::Vector2f xAxis, yAxis;
	public:
		float width, height;
		sf::Vector2f pos;
		sf::Vector2f origin;
	public:
		//angle has to be in degrees!
		OBB(float left, float top, float width, float height, float angle);
		OBB(sf::Vector2f& topLeft, float width, float height, float angle);
		//Local origin
		OBB(float left, float top, float width, float height, float angle, sf::Vector2f origin);
		OBB(sf::Vector2f& topLeft, float width, float height, float angle, sf::Vector2f origin);
		//angle has to be in degrees!
		void setAngle(float newAngle);
		float getAngle() const;
	private:
		OBB() = default;
	};

	class Collider
	{
	public:
		enum class Type
		{
			rect,
			obb,
			circle
		};
	private:
		friend class Physics;

		Type type;
	public:
		union
		{
			sf::FloatRect rect;
			OBB obb;
			FloatCircle circle;
		} collider;

		Collider(sf::FloatRect& rect);
		Collider(OBB& obb);
		Collider(FloatCircle& circle);
		Type GetType() const;
	private:
		Collider();

		bool intersects(const Collider& other) const;
		bool collide(const Collider& other, sf::Vector2f* minTransVec) const;

		void getPointsAxis(sf::Vector2f* points, sf::Vector2f* axis) const;
		sf::Vector2f getProjectionMinMax(const sf::Vector2f* points, const sf::Vector2f& axis, bool isXAxis) const;
	};
private:
	struct PhysicElement
	{
		std::string id;
		std::vector<std::string> collisionIdPointer;
		std::vector<std::string>* collisionIds;
		bool collidersInPointer;
		union
		{
				Collider* collidersPointer;
				Collider collidersValue;
		} colliders;

		Collider* getCollider() const;
	};
public:
	class Body
	{
		friend class Physics;

		enum class TriggerBodyPart
		{
			NONE,
			HEAD,
			SHOES,
			LEFT,
			RIGHT
		};

		struct TriggerInformation
		{
			std::string triggerElementCollision = "";
			TriggerBodyPart triggerBodyPart = TriggerBodyPart::NONE;
		};

		bool isStatic;
		bool isTrigger;
		bool triggered = false;
		TriggerInformation triggerInformation = {};
		sf::Vector2f pos;
		PhysicElement physicsElement;
	public:
		sf::Vector2f vel = { 0.0f, 0.0f };
	public:
		//Should be called, if the object is moving
		Body(sf::Vector2f& pos, std::string name, Collider* collider, std::vector<std::string>* collisionId, bool isTrigger = false, bool isStatic = false);
		Body(sf::Vector2f& pos, std::string name, Collider* collider, bool isTrigger = false, bool isStatic = false, std::vector<std::string> collisionId = {});
		//Should be called if the object, is a static one
		Body(std::string name, Collider collider, bool isTrigger = false, bool isStatic = true, std::vector<std::string> collisionId = {});
		bool getIsTriggerd();
		sf::Vector2f& getPos();
		void setPos(sf::Vector2f newPos);
		TriggerInformation& getTriggerInformation(); //TODO: Make this a callback thing and therefore then remove triggered
		std::string& getId();
	};
private:
	static constexpr float gravity = 9.81f;
	std::unordered_map<std::string, std::shared_ptr<Body>> bodies;
private:
	void handleCollision(std::shared_ptr<Body>& itBody, std::shared_ptr<Body>& collideElementBody, Collider & bodyCollider,
						 const Collider& elementCollider);
public:
	Physics();
	void update(float dt);
	void debugRenderBodies(sf::RenderWindow& window);
	//Use if you need a reference to the body, to get back triggerInformation etc.
	void addElementPointer(std::shared_ptr<Body> body);
	//Use this otherwise
	void addElementValue(Body body);
	bool removeElementById(std::string& id);
};