#ifndef EDITORSYSTEM_H
#define EDITORSYSTEM_H

#include"../ECS/Base/System.h"
#include"../DataStructures/Vector.h"
class EditorSystem final : public System
{
public:
	EditorSystem(const UpdatePriority& updatePriority);

	virtual std::shared_ptr<BaseComponent> DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS) override;

	void SystemDisableProcedure(ECS_Engine& ecs) override;

	const bool GetIsEditorWorking()const { return m_clickFocusOnEntity != -1; };

private:
	void ResetComponent(const uint32_t& entityID, ECS_Engine& ecs) override;
	virtual void UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime) override;

	int32_t
		m_clickFocusOnEntity{ -1 },
		m_backgroundFocus{ -1 };


};

#endif
