#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include"../Base/BaseComponent.h"
#include"../DataStructures/Vector.h"

#include"../GeneralObjects/Graph.h"
struct AIComponent final : BaseComponent
{
	//this will be set from a script then the AI system will safely move an entity to the position
	Vector2f 
		m_destinationPosition{0.f, 0.f},
		m_goalPosition{0.0f, 0.0f};

	float m_walkSpeed{1.0f};


	bool 
		m_usePathfinding{ true },
		m_isInTransit{ false };

	void InitializeAIComponent(const bool& usePathfinding, const float& walkspeed) {
		m_destinationPosition = Vector2f(0.0f, 0.0f);
		m_goalPosition = Vector2f(0.0f, 0.0f);
		m_usePathfinding = usePathfinding;
		m_isInTransit = false;
		m_walkSpeed = walkspeed;
	};


};

#endif