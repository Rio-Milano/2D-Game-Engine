#include "RenderSystem.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ECS_Engine.h"
#include<iostream>
#include<string>

RenderSystem::RenderSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<RenderComponent>();
}

void RenderSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
	//we may not have a render coponent
	std::shared_ptr<RenderComponent> renderComponent{std::static_pointer_cast<RenderComponent>(m_componentEngine.GetComponent(entityID))};
	if (!renderComponent)
	{
		std::cerr << std::string("Render Component of entity :" + std::to_string(entityID) + " was not found!\n");
		return;
	}
	renderComponent->m_renderConfigs = static_cast<unsigned int>(RenderComponent::RenderConfigs::NoConfigs);
	renderComponent->m_renderLayer = 0u;
}

std::shared_ptr<BaseComponent> RenderSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	//we may not have a movement component
	std::shared_ptr<RenderComponent> toClone{ std::static_pointer_cast<RenderComponent>(m_componentEngine.GetComponent(entityID)) };

	if (!toClone)
	{
		std::cerr << std::string("Render Component of entity :" + std::to_string(entityID) + " was not found\n");
		return nullptr;
	}


	std::shared_ptr<RenderComponent> duplicatedComponent = forignECS.AddComponent<RenderComponent>(forignEntityID);

	duplicatedComponent->InitializeRenderComponent(
		toClone->m_renderConfigs, toClone->m_renderLayer, toClone->m_isActive, toClone->m_WorldRender
	);
	duplicatedComponent->m_onlyRenderWhenInEditorMode = toClone->m_onlyRenderWhenInEditorMode;

	return duplicatedComponent;

}
