#include"BulletScript.h"

#include"../ECS/Base/ScriptAllocator.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#define ScriptAllocator ScriptAllocator::GetInstance()
void BulletScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
}

void BulletScript::ReInitialize()
{

}

std::shared_ptr<BaseScript> BulletScript::CloneScript()
{
	return ScriptAllocator.GetScript<BulletScript>();
}

void BulletScript::OnCollide(const uint32_t& entityID, ECS_Engine& ecs, ECS_Engine& forignECS, RenderManager& renderManager)
{
	ecs.DeleteEntity(entityID);

	uint32_t newAnimation{ ecs.InstantiateEntity() };
	ecs.LoadEntityFromSystemIntoSystem(newAnimation, forignECS.GetTaggedEntities("Explosion")[0], forignECS);
	renderManager.AddEntityIdToRenderLayer(ecs, newAnimation, ecs.GetComponent<RenderComponent>(newAnimation)->m_renderLayer);

	std::shared_ptr<TransformComponent> newAnimationTransform{ ecs.GetComponent<TransformComponent>(newAnimation) };
	std::shared_ptr<TransformComponent> entityTransform{ecs.GetComponent<TransformComponent>(entityID)};

	newAnimationTransform->position = entityTransform->position;

}
