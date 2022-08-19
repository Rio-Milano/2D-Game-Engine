#include"SilverKnightScript.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../ECS/MySystems/AllSystemHeadders.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../Scenes/SceneCore/Scene.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../Core/Renderer.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()

void SilverKnightScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{

	std::shared_ptr<AIComponent> aiComponent{ecs.GetComponent<AIComponent>(entityID)};
	uint32_t playerID{ ecs.GetTaggedEntities("Player")[0] };
	std::shared_ptr<CollisionComponent> playerCollisionComponent{ ecs.GetComponent<CollisionComponent>(playerID)};
	aiComponent->m_goalPosition = playerCollisionComponent->m_Rect.GetRectCentre();

	AIDirection direction{ ecs.GetSystem<AISystem>()->GetEntityDirection(entityID, ecs) };

	//entity components
	std::shared_ptr<AnimationComponent> entityAnimationComponent{ ecs.GetComponent<AnimationComponent>(entityID) };

	std::shared_ptr<AnimationSystem> animationSystem{ ecs.GetSystem<AnimationSystem>() };

	std::string animName{ "SilverKnight_" };

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



	if (shootCooldown <= 0.0f)
	{
		shootCooldown = 1.0f*(rand()%10+1);

		uint32_t newBullet{ ecs.InstantiateEntity() };
		ecs.LoadEntityFromSystemIntoSystem(newBullet, forignECS.GetTaggedEntities("Bullet")[0], forignECS);

		std::shared_ptr<TransformComponent>	bulletTransform{ ecs.GetComponent<TransformComponent>(newBullet) };
		std::shared_ptr<SpriteComponent>bulletSprite{ ecs.GetComponent<SpriteComponent>(newBullet) };
		std::shared_ptr<MovementComponent> bulletMovement{ ecs.GetComponent<MovementComponent>(newBullet) };
		
		std::shared_ptr<CollisionComponent> playerCollision{ ecs.GetComponent<CollisionComponent>(ecs.GetTaggedEntities("Player")[0])};
		std::shared_ptr<CollisionComponent>	entityCollision{ ecs.GetComponent<CollisionComponent>(entityID) };


		bulletTransform->position = entityCollision->m_Rect.GetRectCentre();
		bulletTransform->position.x -= bulletSprite->m_sprite.m_textureRef->m_textureRect.GetRectWidth() / 2.f;
		bulletTransform->position.y -= bulletSprite->m_sprite.m_textureRef->m_textureRect.GetRectHeight() / 2.f;
		Vector2f V{ (playerCollision->m_Rect.GetRectCentre() - entityCollision->m_Rect.GetRectCentre()).Normalize() };
		bulletTransform->position = bulletTransform->position + V * 70.f;
		bulletMovement->m_force = V * 1000.f;

		ecs.TagEntity("Bullet", newBullet);
		renderManager.AddEntityIdToRenderLayer(ecs, newBullet, ecs.GetComponent<RenderComponent>(newBullet)->m_renderLayer);

		SoundManager.PlaySound("GunFire");
	}
	if (shootCooldown > 0.0f)
		shootCooldown -= 1 / 10.f;

}



void SilverKnightScript::ReInitialize()
{
}

std::shared_ptr<BaseScript> SilverKnightScript::CloneScript()
{
	return ScriptAllocator.GetScript<SilverKnightScript>();
}

void SilverKnightScript::OnDeath(ECS_Engine& ecs)
{
	std::static_pointer_cast<PlayerScript>(ecs.GetComponent<ScriptComponent>(ecs.GetTaggedEntities("Player")[0])->m_script)->GivePlayerScore(10u);

}


