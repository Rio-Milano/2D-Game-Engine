#ifndef LIFESYSTEM_H
#define LIFESYSTEM_H

#include"../base/System.h"


class LifeSystem final : public System
{

public:
	LifeSystem(const UpdatePriority& updatePriority);

	void ResetComponent(const uint32_t& entityId, ECS_Engine& ecs) override;

	void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime) override;
private:

	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)override;

};

#endif
