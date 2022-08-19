#include"WorldSpawnerScript.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocatpr ScriptAllocator::GetInstance()
#include"../Scenes/SceneCore/Scene.h"
#include"../ECS/Base/Core_ECS_Assets.h"

void WorldSpawnerScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	constexpr static float genericDistanceBetweenSpawner{1000.0f};
	//simple script to turn on/off spawner bassed on players distance to it
		
	std::shared_ptr<CollisionComponent> playerCollisionComponent{ nullptr };

	//get positions of player and spawner
	const std::vector<uint32_t>& players{ ecs.GetTaggedEntities("Player")};
	if (players.size())
		playerCollisionComponent = ecs.GetComponent<CollisionComponent>(players[0]);
	else
		return;

	std::shared_ptr<TransformComponent> spawnerTransfromComponent{ ecs.GetComponent<TransformComponent>(entityID) };

	//find distance between them
	const float distanceBetweenPlayerAndSpawner{Vector2f::Distance(playerCollisionComponent->m_Rect.GetRectCentre(), spawnerTransfromComponent->position)};

	//get the spawner component
	std::shared_ptr<SpawnerComponent> spawnerComponent{ecs.GetComponent<SpawnerComponent>(entityID)};

	//if close enough to spawner
	if (distanceBetweenPlayerAndSpawner <= genericDistanceBetweenSpawner)
		spawnerComponent->m_isSpawnerActive = true;
	//if too far from spawner
	else
		spawnerComponent->m_isSpawnerActive = false;
}


void WorldSpawnerScript::ReInitialize()
{
}

std::shared_ptr<BaseScript> WorldSpawnerScript::CloneScript()
{
	std::shared_ptr<WorldSpawnerScript> newScript{ ScriptAllocatpr.GetScript<WorldSpawnerScript>()};
	return newScript;
}

