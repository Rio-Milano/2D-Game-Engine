#include"BommerScript.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()
#include"../Misc/WorldData.h"
#define WorldData WorldData::GetInstance()


void BommerScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	std::shared_ptr<AIComponent> aiComponent{ ecs.GetComponent<AIComponent>(entityID) };
	uint32_t playerID{ ecs.GetTaggedEntities("Player")[0] };
	std::shared_ptr<CollisionComponent> playerCollisionComponent{ ecs.GetComponent<CollisionComponent>(playerID) };
	aiComponent->m_goalPosition = playerCollisionComponent->m_Rect.GetRectCentre();

	AIDirection direction{ ecs.GetSystem<AISystem>()->GetEntityDirection(entityID, ecs) };

	//entity components
	std::shared_ptr<AnimationComponent> entityAnimationComponent{ ecs.GetComponent<AnimationComponent>(entityID) };

	std::shared_ptr<AnimationSystem> animationSystem{ ecs.GetSystem<AnimationSystem>() };

	std::string animName{ "Bommer_" };

	if (direction == AIDirection::S)
	{
		animName += "WalkForward";
	}
	else if (direction == AIDirection::N)
	{
		animName += "WalkBackward";
	}

	else if (direction == AIDirection::W)
	{
		animName += "WalkLeft";
	}

	else if (direction == AIDirection::E)
	{

		animName += "WalkRight";
	}

	else
	{
		entityAnimationComponent->m_isActive = false;
		return;
	}

	//if animation is playing but not playing correct animation
	if (entityAnimationComponent->m_isActive && entityAnimationComponent->m_animation_name != animName)
		animationSystem->PlayAnimation(entityID, animName, ecs);

	if (!entityAnimationComponent->m_isActive)
	{
		if (entityAnimationComponent->m_animation_name == animName)//if resuming current animation then dont restart it otherwise it will jump between frames
			entityAnimationComponent->m_isActive = true;
		else
			//if not on current animation then play from start
			animationSystem->PlayAnimation(entityID, animName, ecs);
	}

	std::shared_ptr<LifeComponent> life{ ecs.GetComponent<LifeComponent>(entityID) };

	if (life->m_hasTakenDamage)
		SoundManager.PlaySound("EnemyTakingDamage");


	//blow the world here
	
	//get player centre and entity transform
	std::shared_ptr<CollisionComponent> playerCollision{ ecs.GetComponent<CollisionComponent>(ecs.GetTaggedEntities("Player")[0]) };
	std::shared_ptr<TransformComponent>	entityTransform{ ecs.GetComponent<TransformComponent>(entityID) };

	//find the distance between the 2 components
	const float distanceFromPlayer{Vector2f::Distance(playerCollision->m_Rect.GetRectCentre(), entityTransform->position)};

	if (distanceFromPlayer < 100.0f)
	{
		//delete the bommer
		ecs.DeleteEntity(entityID);

		//spawn a bom
		uint32_t newBom{ecs.InstantiateEntity()};
		ecs.LoadEntityFromSystemIntoSystem(newBom, forignECS.GetTaggedEntities("Bom")[0], forignECS);

		//set bom position to entity position
		std::shared_ptr<TransformComponent> newBomTransform{ecs.GetComponent<TransformComponent>(newBom)};
		newBomTransform->position = entityTransform->position - Vector2f(60.f, 60.f);

		//display bom
		renderManager.AddEntityIdToRenderLayer(ecs, newBom, ecs.GetComponent<RenderComponent>(newBom)->m_renderLayer);

		ecs.GetComponent<LifeComponent>(ecs.GetTaggedEntities("Player")[0])->m_health -= 30 * WorldData.DamageMultiplyer;

		SoundManager.PlaySound("BombSound");
	}



}

void BommerScript::ReInitialize()
{
}

std::shared_ptr<BaseScript> BommerScript::CloneScript()
{
	return ScriptAllocator.GetScript<BommerScript>();
}

void BommerScript::OnDeath(ECS_Engine& ecs)
{
	std::static_pointer_cast<PlayerScript>(ecs.GetComponent<ScriptComponent>(ecs.GetTaggedEntities("Player")[0])->m_script)->GivePlayerScore(10u);
}
