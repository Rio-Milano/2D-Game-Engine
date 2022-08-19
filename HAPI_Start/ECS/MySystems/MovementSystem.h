#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include"../Base/System.h"
#include"../MyComponents/MovementComponent.h"


class MovementSystem final : public System
{
public:
	/*
	SEE SYSTEM CLASS FOR COMMENTS
	*/

	MovementSystem(const UpdatePriority& updatePriority);
	~MovementSystem() = default;

	
	/*
	Overriding Virtual Functions
	*/
	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) override;

private:

	void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime) override;
	
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS) override;;
};

#endif