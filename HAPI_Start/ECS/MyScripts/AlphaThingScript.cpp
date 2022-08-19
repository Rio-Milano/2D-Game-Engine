#include "AlphaThingScript.h"

#include"../ECS/MyComponents/MovementComponent.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../UI/UI Core/UI_Manager.h"

#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()

void AlphaThingScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	std::shared_ptr<MovementComponent> movementComponent{ ecs.GetComponent<MovementComponent>(entityID) };
	
	movementComponent->m_force.x = static_cast<float>(rand() % 3 - 1) * m_alphaThingVelocity;
	movementComponent->m_force.y = static_cast<float>(rand() % 3 - 1) * m_alphaThingVelocity;
}

void AlphaThingScript::ReInitialize()
{
}

std::shared_ptr<BaseScript> AlphaThingScript::CloneScript()
{
	std::shared_ptr<AlphaThingScript> newScript{ScriptAllocator.GetScript<AlphaThingScript>()};
	newScript->InitializeScript(this->m_alphaThingVelocity);
	return newScript;
}
