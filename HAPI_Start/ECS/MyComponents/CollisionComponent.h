#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H


#include"../ECS/Base/BaseComponent.h"
#include"../GeneralObjects/Rect.h"

struct CollisionComponent final : public BaseComponent
{
	typedef enum CollisionComponentCollisionLayer
	{
		Default = 0,
		Layer1 = 1<<0,
		Layer2 = 1<<1,
		Layer3 = 1<<2,
		Layer4 = 1<<3,
	}CollisionLayer;

	void InitializeCollisionComponent(const unsigned int& collisionLayer, const bool& isActive, const FRect& rect, const bool& isDynamicCollidable = false)
	{
		m_collisionLayer=collisionLayer;
		m_isActive=isActive; 
		m_Rect=rect;
		m_isDynamicCollidable = isDynamicCollidable;
	}

	~CollisionComponent() = default;



	unsigned int m_collisionLayer;
	bool m_isActive{true};
	int32_t m_CollidingWith{-1};
	FRect m_Rect;
	bool m_isDynamicCollidable{false};

};

#endif