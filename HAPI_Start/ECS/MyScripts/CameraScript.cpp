#include "CameraScript.h"

#include"../Managers/InputManager.h"
#define InputManager InputManager::GetInstance()
#define Keyboard InputManager.GetKeyboardData()
#define Controller InputManager.GetControllerData()

#include"../ECS/MyComponents/AllComponentHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../Scenes/SceneCore/Scene.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()

void CameraScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	if (!Scene::m_editorMode)
	{
		const std::vector<uint32_t>& players{ ecs.GetTaggedEntities("Player")};
		if (players.size())
		{
			std::shared_ptr<TransformComponent>
				playerTransform{ ecs.GetComponent<TransformComponent>(players[0]) },
				cameraTransform{ ecs.GetComponent<TransformComponent>(entityID) };
			
			std::shared_ptr<SpriteComponent> playerSprite{ecs.GetComponent<SpriteComponent>(players[0])};


			cameraTransform->position = playerTransform->position;
				
			cameraTransform->position.x -= static_cast<float>(Renderer::m_ScreenWidth / 2) - playerSprite->m_sprite.m_textureRef->m_textureSize.x / 2.0f;
			cameraTransform->position.y -= static_cast<float>(Renderer::m_ScreenHeight / 2) - playerSprite->m_sprite.m_textureRef->m_textureSize.y / 2.0f;
		}
		else
		{
			//get camera movement component
			std::shared_ptr<MovementComponent> cameraMovementComponent{ ecs.GetComponent<MovementComponent>(entityID) };

			cameraMovementComponent->m_force.y = 0;
			cameraMovementComponent->m_force.x = 0;


			if (Keyboard.scanCode[HK_DOWN] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				cameraMovementComponent->m_force.y = cameraMovementComponent->m_velocity;

			else if (Keyboard.scanCode[HK_UP] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				cameraMovementComponent->m_force.y = -cameraMovementComponent->m_velocity;


			else if (Keyboard.scanCode[HK_LEFT] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				cameraMovementComponent->m_force.x = -cameraMovementComponent->m_velocity;


			else if (Keyboard.scanCode[HK_RIGHT] || Controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
				cameraMovementComponent->m_force.x = cameraMovementComponent->m_velocity;


		}
	}

	/*HAPI_TControllerData kbd{ HAPI.GetControllerData(0) };
	if (kbd.isAttached)
	{
		if (kbd.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			cameraTransformComponent->position.y -= m_CamersSpeed.y * deltaTime;

		else if (kbd.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			cameraTransformComponent->position.y += m_CamersSpeed.y * deltaTime;

		else if (kbd.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] > HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			cameraTransformComponent->position.x += m_CamersSpeed.x * deltaTime;

		else if (kbd.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X] < -HK_GAMEPAD_LEFT_THUMB_DEADZONE)
			cameraTransformComponent->position.x -= m_CamersSpeed.x * deltaTime;
	}*/
}

void CameraScript::ReInitialize()
{

}

std::shared_ptr<BaseScript> CameraScript::CloneScript()
{

	return ScriptAllocator.GetScript<CameraScript>();
}
