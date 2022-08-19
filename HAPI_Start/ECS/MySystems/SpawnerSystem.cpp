#include"SpawnerSystem.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Base/ECS_Engine.h"
#include"../Scenes/SceneCore/Scene.h"

SpawnerSystem::SpawnerSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<SpawnerComponent>();
}

void SpawnerSystem::ResetComponent(const uint32_t& entityId, ECS_Engine& ecs)
{
}

void SpawnerSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	std::shared_ptr<SpawnerComponent> spawnerComponent{ ecs.GetComponent<SpawnerComponent>(entityID) };

	if (spawnerComponent->m_isSpawnerActive)
	{
		if (spawnerComponent->m_spawnAmount > 0)
		{
			if (!Scene::m_editorMode)
			{
				if (spawnerComponent->m_entityTagName_ofEntityToBeSpawned != "")
				{
					if (spawnerComponent->m_spawnFrequencyCount >= 1.0f)
					{

						spawnerComponent->m_spawnFrequencyCount = 0.0f;
						spawnerComponent->m_spawnAmount--;

						//spawn the entity
						uint32_t newEntity{ ecs.InstantiateEntity() };

						ecs.LoadEntityFromSystemIntoSystem(newEntity, forignEcs.GetTaggedEntities(spawnerComponent->m_entityTagName_ofEntityToBeSpawned)[0], forignEcs);

						std::shared_ptr<TransformComponent> spawnerTransform{ ecs.GetComponent<TransformComponent>(entityID) };
						std::shared_ptr<TransformComponent> newEntityTransform{ ecs.GetComponent<TransformComponent>(newEntity) };


						newEntityTransform->position = spawnerTransform->position;

						renderManager.AddEntityIdToRenderLayer(ecs, newEntity, ecs.GetComponent<RenderComponent>(newEntity)->m_renderLayer);

					}

					spawnerComponent->m_spawnFrequencyCount += spawnerComponent->m_spawnFrequency;
				}
			}
		}
	}
}

std::shared_ptr<BaseComponent> SpawnerSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	std::shared_ptr<SpawnerComponent> toClone{std::static_pointer_cast<SpawnerComponent>(m_componentEngine.GetComponent(entityID))};

	std::shared_ptr<SpawnerComponent> cloned{forignECS.AddComponent<SpawnerComponent>(forignEntityID)};

	cloned->InitializeSpawnerComponent(toClone->m_entityTagName_ofEntityToBeSpawned, toClone->m_spawnAmount, toClone->m_spawnFrequency);

	return cloned;
}
