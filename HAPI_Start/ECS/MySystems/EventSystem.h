
#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include"../ECS/Base/System.h"

class EventSystem final : public System
{
public:
	EventSystem(const UpdatePriority& updatePriorityRef);
	~EventSystem() = default;

	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) override;

	void EndOfSystemUpdate() override;

private:
	void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)override;
	
	std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS) override;

	bool m_hasFoundEntity{false};
	bool m_hasEngagedWithEntity{false};
};

#endif