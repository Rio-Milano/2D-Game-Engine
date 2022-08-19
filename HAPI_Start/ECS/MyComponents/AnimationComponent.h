#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include"../Base/BaseComponent.h"
#include<string>
#include<memory>

struct AnimationComponent final : public BaseComponent
{
	void InitializeAnimationComponent
	(
		const std::string& animationName,
		const unsigned int& animationFrame,
		const float& speedArg,
		const bool& isActive,
		const bool& looping,
		const bool& destroyOnFinish
	)
	{
		m_animationFrame = animationFrame,
		m_animation_name = animationName;
		speed = speedArg;
		m_isActive = isActive;
		m_Looping = looping;
		m_DestroyOnFinish =  destroyOnFinish;
	}
	
	unsigned int 
		m_animationFrame{0U};
	
	std::string 
		m_animation_name{""};
	
	bool
		m_isActive{ false },
		m_Looping{ false },
		m_hasFinished{false},
		m_DestroyOnFinish{false};
	
	float 
		speed{0.0f},
		elapsed{ 0.0f };

};

#endif