#include"LifeSystem.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../ECS/Base/ECS_Engine.h"

LifeSystem::LifeSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<LifeComponent>();
}

void LifeSystem::ResetComponent(const uint32_t& entityId, ECS_Engine& ecs)
{
	std::shared_ptr<LifeComponent> life{ ecs.GetComponent<LifeComponent>(entityId) };
	life->m_applyDamage = 0.0f;
	life->m_canTakeDamage = false;
	life->m_health = 100.0f;
	life->m_maxHealth = 100.0f;
}

void LifeSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	std::shared_ptr<LifeComponent> life{ ecs.GetComponent<LifeComponent>(entityID) };
	if (life->m_destroyOnDeath)
	{
		if (life->m_health <= 0.0f)
		{
			ecs.DeleteEntity(entityID);
			std::shared_ptr<ScriptComponent> script{ ecs.GetComponent<ScriptComponent>(entityID) };
			if (script != nullptr)
				script->m_script->OnDeath(ecs);
		}
	}
	life->m_hasTakenDamage = false;
}

std::shared_ptr<BaseComponent> LifeSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	std::shared_ptr<LifeComponent> toClone{std::static_pointer_cast<LifeComponent>(m_componentEngine.GetComponent(entityID))};

	std::shared_ptr<LifeComponent> cloned{forignECS.AddComponent<LifeComponent>(forignEntityID)};

	cloned->InitializeLifeComponent(toClone->m_canTakeDamage, toClone->m_health, toClone->m_maxHealth, toClone->m_applyDamage, toClone->m_destroyOnDeath, toClone->m_drawHealthBar);

	return cloned;
}
