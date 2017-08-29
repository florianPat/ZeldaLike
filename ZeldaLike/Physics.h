#pragma once

#include "SFML\Graphics.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

//TODO: Add OBB, Circle, Line Intersection + PhysicsElementGroup

class Physics
{
	struct PhysicElement
	{
		std::string id;
		std::shared_ptr<std::vector<std::string>> collisionIdPointer; //NOTE: Used shared-ptr here, because this should be copyable...
		std::vector<std::string>* collisionIds;
		bool collidersInPointer;
		sf::FloatRect shoes;
		sf::FloatRect head;
		sf::FloatRect leftBody, rightBody;
		struct //TODO: Make this a union, if possible! ( First attempt was a fail ;) )
		{
			sf::FloatRect* collidersPointer;
			sf::FloatRect collidersValue;
		} colliders;
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
	private:
		void updateShoes();
		void updateLeftRightBody();
		void updateHead();
	public:
		//Should be called, if the object is moving
		Body(sf::Vector2f& pos, std::string name, sf::FloatRect* collider, std::vector<std::string>* collisionId, bool isTrigger = false, bool isStatic = false);
		Body(sf::Vector2f& pos, std::string name, sf::FloatRect* collider, bool isTrigger = false, bool isStatic = false, std::vector<std::string> collisionId = {});
		//Should be called if the object, is a static one
		Body(std::string name, sf::FloatRect collider, bool isTrigger = false, bool isStatic = true, std::vector<std::string> collisionId = {});
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
	void handleCollision(std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& it,
		std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, std::shared_ptr<Body>>>>>& collideElement,
		sf::FloatRect& bodyRect, const sf::FloatRect& elementRect);
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