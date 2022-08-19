#include "PlayerScript.h"

#include"../Managers/AllManagerHeadders.h"
#include"../ECS/MyComponents/AllComponentHeadders.h"
#include"../ECS/MySystems/AllSystemHeadders.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../Scenes/SceneCore/Scene.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../Core/Renderer.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()
#include"../Managers/SoundManager.h"
#define SoundManager SoundManager::GetInstance()


void PlayerScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	if (!Scene::m_editorMode)
	{
		UpdateDynamics(ecs, entityID);
		UpdatePosition(ecs, entityID);
		UpdateCollision(ecs, entityID);


		static float shootCooldown{0.0f};

		if(Mouse.leftButtonDown && shootCooldown <= 0)
		{
			shootCooldown = 1.0f;

			uint32_t newBullet{ ecs.InstantiateEntity() };
			ecs.LoadEntityFromSystemIntoSystem(newBullet, forignECS.GetTaggedEntities("Bullet")[0], forignECS);
			
			std::shared_ptr<TransformComponent>	bulletTransform{ ecs.GetComponent<TransformComponent>(newBullet) };
			std::shared_ptr<SpriteComponent>bulletSprite{ ecs.GetComponent<SpriteComponent>(newBullet) };
			std::shared_ptr<MovementComponent> bulletMovement{ecs.GetComponent<MovementComponent>(newBullet)};
			std::shared_ptr<CollisionComponent>	entityCollision{ecs.GetComponent<CollisionComponent>(entityID) };
			std::shared_ptr<LifeComponent> lifeComponent{ecs.GetComponent<LifeComponent>(newBullet)};

			bulletTransform->position = entityCollision->m_Rect.GetRectCentre();
			bulletTransform->position.x -= bulletSprite->m_sprite.m_textureRef->m_textureRect.GetRectWidth() / 2.f;
			bulletTransform->position.y -= bulletSprite->m_sprite.m_textureRef->m_textureRect.GetRectHeight() / 2.f;
			Vector2f V{ (RenderManager::mousePositionInWorldForTestingAIPathfindingInDevTesting.ToType<float>() - entityCollision->m_Rect.GetRectCentre()).Normalize() };
			bulletTransform->position = bulletTransform->position + V * 40.f;
			bulletMovement->m_force = V * 1000.f;
			lifeComponent->m_applyDamage = 50.f;

			ecs.TagEntity("Bullet", newBullet);
			renderManager.AddEntityIdToRenderLayer(ecs, newBullet, ecs.GetComponent<RenderComponent>(newBullet)->m_renderLayer);

			SoundManager.PlaySound("GunFire");
		}
		if (shootCooldown > 0.0f)
			shootCooldown -= 1 / 10.f;
		
	}
	//if (ecs.GetComponent<LifeComponent>(entityID)->m_health )
	//	renderManager.RemoveEntityIdFromRenderLayer(entityID, ecs.GetComponent<RenderComponent>(entityID)->m_renderLayer);
	
}

void PlayerScript::ReInitialize()
{
}

std::shared_ptr<BaseScript> PlayerScript::CloneScript()
{
	m_positionSet = false;
	return ScriptAllocator.GetScript<PlayerScript>();
}

void PlayerScript::GivePlayerScore(const unsigned int& score)
{
	m_playerScore += score;
}

void PlayerScript::UpdateDynamics(ECS_Engine& ecs, const uint32_t& entityID)
{
	//entity components
	std::shared_ptr<AnimationComponent> entityAnimationComponent{ ecs.GetComponent<AnimationComponent>(entityID) };
	std::shared_ptr<MovementComponent> entityMovementComponent{ecs.GetComponent<MovementComponent>(entityID)};
	
	entityMovementComponent->m_force = Vector2f(0.0f, 0.0f);

	std::shared_ptr<AnimationSystem> animationSystem{ ecs.GetSystem<AnimationSystem>() };

	std::string animName{ "Player_Walk_" };

	if (Keyboard.scanCode[HK_DOWN] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		animName += "Forward";
		entityMovementComponent->m_force.y = entityMovementComponent->m_velocity;
	}
	else if (Keyboard.scanCode[HK_UP] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		animName += "Back";
		entityMovementComponent->m_force.y = -entityMovementComponent->m_velocity;
	}

	else if (Keyboard.scanCode[HK_LEFT] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		animName += "Left";
		entityMovementComponent->m_force.x = -entityMovementComponent->m_velocity;
	}

	else if (Keyboard.scanCode[HK_RIGHT] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
	{

		animName += "Right";
		entityMovementComponent->m_force.x = entityMovementComponent->m_velocity;
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

	if (ecs.GetComponent<LifeComponent>(entityID)->m_hasTakenDamage)
		SoundManager.PlaySound("EnemyTakingDamage");
}

void PlayerScript::UpdatePosition(ECS_Engine& ecs, const uint32_t& entityID)
{


	std::shared_ptr<TransformComponent> entityTransformComponent{ ecs.GetComponent<TransformComponent>(entityID) };

	if (!this->m_positionSet)
	{
		const std::vector<uint32_t>& placementPersons{ecs.GetTaggedEntities("PersonPlacement")};

		if (placementPersons.size())
		{
			const uint32_t& placementPersonID{ placementPersons[0] };

			std::shared_ptr<TransformComponent> personTransform{ecs.GetComponent<TransformComponent>(placementPersonID)};
			std::shared_ptr<SpriteComponent> personSprite{ecs.GetComponent<SpriteComponent>(placementPersonID)};

			std::shared_ptr<TransformComponent> playerTransform{ecs.GetComponent<TransformComponent>(entityID)};
			std::shared_ptr<SpriteComponent> playerSprite{ecs.GetComponent<SpriteComponent>(entityID)};

			playerTransform->position =
				personTransform->position +
				personSprite->m_sprite.m_textureRef->m_textureSize.ToType<float>() / 2.f -
				playerSprite->m_sprite.m_textureRef->m_textureSize.ToType<float>() / 2.f;

				std::shared_ptr<CollisionComponent> collisionComponent{ecs.GetComponent<CollisionComponent>(entityID)};
				collisionComponent->m_Rect.Translate(
					entityTransformComponent->position + personSprite->m_sprite.m_textureRef->m_textureSize.ToType<float>()/4.0f
				);

				collisionComponent->m_Rect.m_offset.y += 5.f;

		}
		m_positionSet = true;
	}

}


void PlayerScript::UpdateCollision(ECS_Engine& ecs, const uint32_t& entityID)
{

}

