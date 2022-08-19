#ifndef LIFECOMPONENT_H
#define LIFECOMPONENT_H

#include"../ECS/Base/BaseComponent.h"

struct LifeComponent final : public BaseComponent
{
	bool
		m_canTakeDamage{ false },
		m_destroyOnDeath{ true },
		m_hasTakenDamage{false},
		m_drawHealthBar{true};
	/*
	Could add a life span attribute here
	*/

	float
		m_health{100.0f},
		m_maxHealth{100.0f},
		m_applyDamage{0.0f};

	void InitializeLifeComponent(const bool& canTakeDamage, const float& health, const float& maxHealth, const float& applyDamage, const bool& destroyOnDeath, const bool& drawHealthBar)
	{
		m_canTakeDamage = canTakeDamage;
		m_health = health;
		m_maxHealth = maxHealth;
		m_applyDamage = applyDamage;
		m_destroyOnDeath = destroyOnDeath;
		m_drawHealthBar = drawHealthBar;
	};
};

#endif