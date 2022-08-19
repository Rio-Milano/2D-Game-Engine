
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../Base/ECS_Engine.h"
#include"../Misc/WorldData.h"
#include"../Misc/WorldData.h"
#define WorldData WorldData::GetInstance()
#include<iostream>
#include<string>

MovementSystem::MovementSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<MovementComponent>();
}

void MovementSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	//we have a movementComponent
	std::shared_ptr<MovementComponent> movementComponent{std::static_pointer_cast<MovementComponent>(m_componentEngine.GetComponent(entityID))};

	//we may not have a transform component
	std::shared_ptr<TransformComponent> transformComponent = ecs.GetComponent<TransformComponent>(entityID);
	if (!transformComponent)
	{
		std::cerr << std::string("Transform Component of entity :" + std::to_string(entityID) + " was not found\n");
		return;
	}
	Vector2f prevPos{transformComponent->position};

	transformComponent->position.x += movementComponent->m_force.x *deltaTime;
	transformComponent->position.y += movementComponent->m_force.y *deltaTime;

	std::shared_ptr<CollisionComponent> collisionComponent{ecs.GetComponent<CollisionComponent>(entityID)};
	if (collisionComponent)
	{
		//align the collision component rect with the transform components position
		collisionComponent->m_Rect.Translate(transformComponent->position + collisionComponent->m_Rect.m_offset);

		bool isColliding{ ecs.GetSystem<CollisionSystem>()->IsColliding(entityID, ecs, forignEcs, renderManager) };
		if (isColliding)
		{
			transformComponent->position.x -= movementComponent->m_force.x * deltaTime;
			transformComponent->position.y -= movementComponent->m_force.y * deltaTime;
			collisionComponent->m_Rect.Translate(transformComponent->position + collisionComponent->m_Rect.m_offset);

			std::shared_ptr<LifeComponent> entityLife{ecs.GetComponent<LifeComponent>(entityID)};
			std::shared_ptr<LifeComponent> collideWithLife{ecs.GetComponent<LifeComponent>(collisionComponent->m_CollidingWith)};

			if (entityLife != nullptr && collideWithLife != nullptr)
			{
				if (collideWithLife->m_canTakeDamage && entityLife->m_applyDamage > 0.0f)
				{
					collideWithLife->m_health -= entityLife->m_applyDamage * WorldData.DamageMultiplyer;
					collideWithLife->m_hasTakenDamage = true;
				}
			}
		}

	}

}

std::shared_ptr<BaseComponent> MovementSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	//we may not have a movement component
	std::shared_ptr<MovementComponent> toClone{std::static_pointer_cast<MovementComponent>(m_componentEngine.GetComponent(entityID))};

	if (!toClone)
	{
		std::cerr << std::string("Movement Component of entity :" + std::to_string(entityID) + " was not found\n");
		return nullptr;
	}



	std::shared_ptr<MovementComponent> duplicatedComponent = forignECS.AddComponent<MovementComponent>(forignEntityID);

	duplicatedComponent->InitializeMovementComponent(
		toClone->m_force, toClone->m_velocity
	);

	return duplicatedComponent;
}

void MovementSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
	//we may not have a movement component
	std::shared_ptr<MovementComponent> movementComponent = ecs.GetComponent<MovementComponent>(entityID);

	if (!movementComponent)
	{
		std::cerr << std::string("MovementComponent of entity :" + std::to_string(entityID) + " was not found\n");
		return;
	}


}
