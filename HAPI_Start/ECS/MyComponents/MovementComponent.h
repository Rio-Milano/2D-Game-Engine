#ifndef MOVEMENT_H
#define MOVEMENT_H

#include"../Base/BaseComponent.h"
#include"../DataStructures/Vector.h"

struct MovementComponent final : public BaseComponent
{
	explicit MovementComponent(const Vector2f& force = Vector2f(0.0f, 0.0f), const float& velocity = 0.0f)
		:m_force(force),  m_velocity(velocity){}

	void InitializeMovementComponent(const Vector2f& force = Vector2f(0.0f, 0.0f), const float& velocity = 0.0f)
	{
		m_force = force;
		m_velocity = velocity;
	}


	Vector2f m_force{0.0f, 0.0f};
	float m_velocity{1.0f};

	
};

#endif