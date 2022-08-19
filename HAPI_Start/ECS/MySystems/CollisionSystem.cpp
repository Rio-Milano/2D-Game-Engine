#include"CollisionSystem.h"
#include"../Base/ECS_Engine.h"
#include"../MyComponents/AllComponentHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include<string>
#include<iostream>
#include<assert.h>

//private system functions
CollisionSystem::CollisionSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<CollisionComponent>();
}

std::shared_ptr<BaseComponent> CollisionSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	//we may not have a collision component
	std::shared_ptr<CollisionComponent> toClone{std::static_pointer_cast<CollisionComponent>(m_componentEngine.GetComponent(entityID))};
	if (!toClone)
	{
		std::cerr << std::string("Collision Component of entity :" + std::to_string(entityID) + " was not found\n");
		return nullptr;
	}

	std::shared_ptr<CollisionComponent> duplicatedComponent = forignECS.AddComponent<CollisionComponent>(forignEntityID);

	duplicatedComponent->InitializeCollisionComponent(
		toClone->m_collisionLayer, toClone->m_isActive, toClone->m_Rect
	);

	duplicatedComponent->m_isDynamicCollidable = toClone->m_isDynamicCollidable;

	return duplicatedComponent;
}


void CollisionSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	std::shared_ptr<CollisionComponent> currentCollisionComponent{ ecs.GetComponent<CollisionComponent>(entityID) };
	
	std::shared_ptr<TransformComponent> currentTransformComponent{ ecs.GetComponent<TransformComponent>(entityID) };
	assert(currentTransformComponent != nullptr);

	//reset collision component data from last update
	currentCollisionComponent->m_CollidingWith = -1;

	std::shared_ptr<MovementComponent> movemenetComponent{ecs.GetComponent<MovementComponent>(entityID)};
	if (!movemenetComponent)
		currentCollisionComponent->m_Rect.Translate(currentTransformComponent->position + currentCollisionComponent->m_Rect.m_offset);

}

const bool CollisionSystem::IsColliding(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, RenderManager& renderManager)
{
	//get the collision component
	std::shared_ptr<CollisionComponent> collisionComponent{ std::static_pointer_cast<CollisionComponent>(m_componentEngine.GetComponent(entityID)) };
	assert(collisionComponent != nullptr);

	//if current component is not active then check for no collision
	if (collisionComponent->m_isActive == false)
		return false;


	std::shared_ptr<CollisionComponent> currentCollisionComponent{ nullptr };


	for (const std::pair<uint32_t, std::shared_ptr<BaseComponent>>& componentRecord : m_componentEngine.GetComponentPool())
	{
		currentCollisionComponent = std::static_pointer_cast<CollisionComponent>(componentRecord.second);

		if (currentCollisionComponent == nullptr)
			continue;


		//if indexed component is not active then dont check for collision
		if (currentCollisionComponent->m_isActive == false)
			continue;

		//if checking against self then dont check for collision
		if (entityID == componentRecord.first)
			continue;

		//if the current component is not on the same collision layer then dont check for collision
		if (!collisionComponent->m_collisionLayer & currentCollisionComponent->m_collisionLayer)
			continue;

		if (collisionComponent->m_Rect.IsRectIntersecting(currentCollisionComponent->m_Rect))
		{
			collisionComponent->m_CollidingWith = componentRecord.first;
			std::shared_ptr<ScriptComponent> scriptComponent_ofCurrent{ecs.GetComponent<ScriptComponent>(entityID)};
			if (scriptComponent_ofCurrent != nullptr)
				scriptComponent_ofCurrent->m_script->OnCollide(entityID, ecs, forignEcs, renderManager);

			return true;
		}

	}
	return false;

}

const bool CollisionSystem::IsCollidingWithLineSegment(const uint32_t& entityID, ECS_Engine& ecs, float s1_x, float s1_y, float s2_x, float s2_y)
{
	//get the collision component
	std::shared_ptr<CollisionComponent> collisionComponent{ std::static_pointer_cast<CollisionComponent>(m_componentEngine.GetComponent(entityID)) };
	if (!collisionComponent)
	{
		std::cerr << "Collision Component of entity : " + std::to_string(entityID) + " was not found\n";
		return false;
	}

	//if current component is not active then check for no collision
	if (collisionComponent->m_isActive == false)
		return false;


	std::shared_ptr<CollisionComponent> currentCollisionComponent{ nullptr };


	for (const std::pair<uint32_t, std::shared_ptr<BaseComponent>>& componentRecord : m_componentEngine.GetComponentPool()) 
	{
		currentCollisionComponent = ecs.GetComponent<CollisionComponent>(componentRecord.first);

		//if checking against self then dont check for collision
		if (entityID == componentRecord.first)
			continue;

		//if indexed component is not active then dont check for collision
		if (currentCollisionComponent->m_isActive == false)
			continue;


		//if the current component is not on the same collision layer then dont check for collision
		if (!collisionComponent->m_collisionLayer & currentCollisionComponent->m_collisionLayer)
			continue;

		//top left  Vector2f(collisionComponent->m_Rect.x1, collisionComponent->m_Rect.y1)
		//top right Vector2f(collisionComponent->m_Rect.x2, collisionComponent->m_Rect.y1)
		//bottom left  Vector2f(collisionComponent->m_Rect.x1, collisionComponent->m_Rect.y2)
		//bottom right Vector2f(collisionComponent->m_Rect.x2, collisionComponent->m_Rect.y2)

		if (LineIntersectsLine(Vector2f(s1_x, s1_y), Vector2f(s2_x, s2_y), Vector2f(currentCollisionComponent->m_Rect.x1, currentCollisionComponent->m_Rect.y1), Vector2f(currentCollisionComponent->m_Rect.x2, currentCollisionComponent->m_Rect.y1)) ||
			LineIntersectsLine(Vector2f(s1_x, s1_y), Vector2f(s2_x, s2_y), Vector2f(currentCollisionComponent->m_Rect.x2, currentCollisionComponent->m_Rect.y1), Vector2f(currentCollisionComponent->m_Rect.x2, currentCollisionComponent->m_Rect.y2)) ||
			LineIntersectsLine(Vector2f(s1_x, s1_y), Vector2f(s2_x, s2_y), Vector2f(currentCollisionComponent->m_Rect.x1, currentCollisionComponent->m_Rect.y2), Vector2f(currentCollisionComponent->m_Rect.x2, currentCollisionComponent->m_Rect.y2)) ||
			LineIntersectsLine(Vector2f(s1_x, s1_y), Vector2f(s2_x, s2_y), Vector2f(currentCollisionComponent->m_Rect.x1, currentCollisionComponent->m_Rect.y1), Vector2f(currentCollisionComponent->m_Rect.x1, currentCollisionComponent->m_Rect.y2)) ||
			(collisionComponent->m_Rect.IsPointLyingOnRect(Vector2f(s1_x, s1_y)) && currentCollisionComponent->m_Rect.IsPointLyingOnRect(Vector2f(s2_x, s2_y))))
			return true;
	}
	return false;
}



bool CollisionSystem::LineIntersectsLine(Vector2f l1p1, Vector2f l1p2, Vector2f l2p1, Vector2f l2p2)
{
	{
		float q = (l1p1.y - l2p1.y) * (l2p2.x - l2p1.x) - (l1p1.x - l2p1.x) * (l2p2.y - l2p1.y);
		float d = (l1p2.x - l1p1.x) * (l2p2.y - l2p1.y) - (l1p2.y - l1p1.y) * (l2p2.x - l2p1.x);

		if (d == 0)
		{
			return false;
		}

		float r = q / d;

		q = (l1p1.y - l2p1.y) * (l1p2.x - l1p1.x) - (l1p1.x - l2p1.x) * (l1p2.y - l1p1.y);
		float s = q / d;

		if (r < 0 || r > 1 || s < 0 || s > 1)
		{
			return false;
		}

		return true;
	}
}

void CollisionSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{

}