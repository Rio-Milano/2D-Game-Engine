#include "AISystem.h"
#include"../Base/ECS_Engine.h"
#include"../MyComponents/AllComponentHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../GeneralObjects/Node.h"
#include"CollisionSystem.h"
#include"TransformSystyem.h"
#include"../Managers/RenderManager.h"
#include<string>
#include<iostream>


AISystem::AISystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<AIComponent>();

}

void AISystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
}

void AISystem::StartOfSystemUpdate()
{
	//clear any temp locked nodes
	for (std::vector<std::shared_ptr<Node>>& graphRow : this->m_graph.GetNodeGraph())
	{
		for (std::shared_ptr<Node> node : graphRow)
		{
			node->m_collideWithEntity = -1;
		}
	}
}

AIDirection AISystem::GetEntityDirection(const uint32_t entityID, ECS_Engine& ecs) const
{
	assert(ecs.GetComponent<CollisionComponent>(entityID) != nullptr);
	assert(ecs.GetComponent<AIComponent>(entityID) != nullptr);

	if(!ecs.GetComponent<AIComponent>(entityID)->m_isInTransit)
		return AIDirection::ZERO;

	Vector2f
		aiCentre{ ecs.GetComponent<CollisionComponent>(entityID)->m_Rect.GetRectCentre() },
		aiDestination{ecs.GetComponent<AIComponent>(entityID)->m_destinationPosition};

	float
		dx{ aiDestination.x - aiCentre.x },
		dy{aiDestination.y - aiCentre.y};


	if(abs(dx) > abs(dy))
	{
		if (dx < 0)
			return AIDirection::W;
		else
			return AIDirection::E;
	}
	else
	{
		if (dy < 0)
			return AIDirection::N;
		else
			return AIDirection::S;
	}

	
}

void AISystem::InitialisePathfindingGraph(const float& graphDensity, const std::shared_ptr<CollisionSystem>& collisionSystem)
{
	//get the world bounds
	this->m_graph.SetGraphMeta(collisionSystem->GetWorldBounds<float>(), graphDensity);
	//inti graph
	this->m_graph.InstansiateGraph();
	//lock the nodes that cant be used
	this->m_graph.PermanentlyLockStaticCollidables(collisionSystem);

}

void AISystem::ClearGraph()
{
	this->m_graph.Clear();
}

void AISystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	if (!this->m_graph.GetNodeGraph().size())
		return;

	//might not have collision component
	std::shared_ptr<CollisionComponent> collisionComponent{ ecs.GetComponent<CollisionComponent>(entityID) };
	assert(collisionComponent != nullptr);

	//if collision component rect can "move"
	if (collisionComponent->m_isDynamicCollidable)
	{
		std::shared_ptr<AIComponent> aiComponent{ ecs.GetComponent<AIComponent>(entityID) };
		if (aiComponent->m_usePathfinding)
		{
			//loop nodes
			for (std::vector<std::shared_ptr<Node>>& graphRow : this->m_graph.GetNodeGraph())
			{
				for (std::shared_ptr<Node>& node : graphRow)
				{
					//if node world position collides with entity collision rect
					if (collisionComponent->m_Rect.IsPointLyingOnRect(node->m_worldPosition))
					{
						if (node->m_collideWithEntity == -1)
							node->m_collideWithEntity = static_cast<int32_t>(entityID);
					}
				}
			}
		}
	}
}

void AISystem::SeccondUpdateComponent(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	if (!this->m_graph.GetNodeGraph().size())
		return;

	//might not have collision component
	std::shared_ptr<CollisionComponent> collisionComponent{ ecs.GetComponent<CollisionComponent>(entityID) };
	assert(collisionComponent != nullptr);

	//if collision component rect can "move"
	if (collisionComponent->m_isDynamicCollidable)
	{
		std::shared_ptr<AIComponent> aiComponent{ ecs.GetComponent<AIComponent>(entityID) };
		if (aiComponent->m_usePathfinding)
		{
			Vector2f startPosition{ collisionComponent->m_Rect.GetRectCentre() };

			std::shared_ptr<MovementComponent> movementComponent{ ecs.GetComponent<MovementComponent>(entityID) };
			assert(movementComponent);
			movementComponent->m_force = Vector2f(0.f, 0.f);

			if (!aiComponent->m_isInTransit)
			{
				std::vector<Vector2f> path{ this->m_graph.GetNextPointInPath(startPosition, aiComponent->m_goalPosition, entityID, ecs, forignEcs, renderManager) };
				if (path.size() > 0)
				{
					aiComponent->m_destinationPosition = path.back();
					aiComponent->m_isInTransit = true;
				}

			}

			if (aiComponent->m_isInTransit)
			{
				Vector2f toNodePointNormal{ (aiComponent->m_destinationPosition - collisionComponent->m_Rect.GetRectCentre()).Normalize() };			

				float distanceBetweenStartPointAndGoalPoint{ Vector2f::Distance(aiComponent->m_destinationPosition, startPosition) };
				if (distanceBetweenStartPointAndGoalPoint < 5.f)
				{
					aiComponent->m_isInTransit = false;
				}
				else
					movementComponent->m_force = movementComponent->m_force + toNodePointNormal * aiComponent->m_walkSpeed;
			}
		}

	}
}

std::shared_ptr<BaseComponent> AISystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	//we may not have an event component
	std::shared_ptr<AIComponent> toClone{ std::static_pointer_cast<AIComponent>(m_componentEngine.GetComponent(entityID)) };
	assert(toClone != nullptr);



	std::shared_ptr<AIComponent> duplicatedComponent = forignECS.AddComponent<AIComponent>(forignEntityID);

	duplicatedComponent->InitializeAIComponent(toClone->m_usePathfinding, toClone->m_walkSpeed);

	return duplicatedComponent;
}
