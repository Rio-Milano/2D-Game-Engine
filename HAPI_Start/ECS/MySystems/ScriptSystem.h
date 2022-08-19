#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include"../ECS/Base/System.h"

class ScriptSystem final : public System
{
public:
	explicit ScriptSystem(const UpdatePriority& updatePriority);

	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)override;

private:
	void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)override;
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS) override;
};

#endif