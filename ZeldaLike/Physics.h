#pragma once

#include "SFML\Graphics.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

//TODO: Add PhysicsElementGroup

class Physics
{
public:
	struct OBB
	{
		friend class Physics;

		float angle;
		float width, height;
		sf::Vector2f xAxis, yAxis;
		sf::Vector2f origin;

		OBB(float left, float top, float width, float height, float angle);
		OBB(sf::Vector2f& topLeft, float width, float height, float angle);
	private:
		OBB() = default;
	};

	class Collider
	{
		friend class Physics;
	private:
		enum class Type
		{
			rect,
			obb
		} type;
	public:
		union
		{
			sf::FloatRect rect;
			OBB obb;
		} collider;

		Collider(sf::FloatRect& rect);
		Collider(OBB& obb);

		bool intersects(const Collider& other) const;
	private:
		Collider();

		void getPointsAxis(sf::Vector2f* points, sf::Vector2f* axis) const;
		void getProjection(float* proj, const sf::Vector2f* points, const sf::Vector2f* axis1, const sf::Vector2f* axis2) const;
		sf::Vector2f getMinMax(const float* proj) const;
	};
private:
	struct PhysicElement
	{
		std::string id;
		std::vector<std::string> collisionIdPointer;
		std::vector<std::string>* collisionIds;
		bool collidersInPointer;
		Collider head, leftBody, rightBody, shoes;
		union
		{
				Collider* collidersPointer;
				Collider collidersValue;
		} colliders;

		Collider* getCollider() const;

		void initBodyParts();
		void updateBodyParts();
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