
#include"TransformSystyem.h"
#include"../MyComponents/TransformComponent.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../Base/ECS_Engine.h"
#include<iostream>
#include<string>

TransformSystem::TransformSystem(const UpdatePriority& updatePriority)
 	:System(updatePriority)
{

	m_componentEngine.AllocateComponents<TransformComponent>();
}

void TransformSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
	std::shared_ptr<TransformComponent> transformComponent{ ecs.GetComponent<TransformComponent>(entityID) };
	if (!transformComponent)
	{
		std::cerr << "transform Component of entity :" + std::to_string(entityID) + " was not found\n";
		return;
	}

	transformComponent->position = {0u, 0u};
	transformComponent->rotation = 0u;
	transformComponent->scale = 0u;

}

std::shared_ptr<BaseComponent> TransformSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	const std::shared_ptr<TransformComponent>& toClone{
		std::static_pointer_cast<TransformComponent>(
				m_componentEngine.GetComponent(entityID)
			)
	};
	if (!toClone)
	{
		std::cerr << "Transform Component of entity :" + std::to_string(entityID) + " was not found\n";
		return nullptr;
	}

	std::shared_ptr<TransformComponent> duplicatedComponent = forignECS.AddComponent<TransformComponent>(forignEntityID);

	duplicatedComponent->InitializeTransformComponent(
		toClone->position,
		toClone->rotation,
		toClone->scale
		);

	return duplicatedComponent;
}
