#include "ScriptSystem.h"
#include"../ECS/MyComponents/ScriptComponent.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ECS_Engine.h"

ScriptSystem::ScriptSystem(const UpdatePriority& updatePriority)
    :System(updatePriority)
{
    m_componentEngine.AllocateComponents<ScriptComponent>();
}

void ScriptSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
    ecs.GetComponent<ScriptComponent>(entityID)->m_script->ReInitialize();
}

void ScriptSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
    ecs.GetComponent<ScriptComponent>(entityID)->m_script->FixedUpdate(ecs, forignEcs, uiManager, renderManager, entityID);
}

std::shared_ptr<BaseComponent> ScriptSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
    std::shared_ptr<ScriptComponent> toClone{std::static_pointer_cast<ScriptComponent>(this->m_componentEngine.GetComponent(entityID))};


    std::shared_ptr<ScriptComponent> duplicatedComponent = forignECS.AddComponent<ScriptComponent>(forignEntityID);

    duplicatedComponent->InitializeScriptComponent(toClone->m_script->CloneScript());

    return duplicatedComponent;

}
