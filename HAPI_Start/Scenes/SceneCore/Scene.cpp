#include"Scene.h"
#include<HAPI_lib.h>
#include"../ECS/MyScripts/CameraScript.h"
#include"../ECS/MyComponents/AllComponentHeadders.h"
#include"../Managers/AllManagerHeadders.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()
ECS_Engine Scene::m_prefabEngine{};
bool Scene::m_editorMode{false};

Scene::Scene(Renderer& renderer, const Scene_Enum& sceneEnum) : 
	m_sceneEnum(sceneEnum), 
	m_rendererReferance(renderer),
	m_UI_Manager(m_renderManager, m_ecsEngine)
{
	InitCamera();
}

void Scene::InitCamera()
{
	uint32_t cameraID{ m_ecsEngine.InstantiateEntity() };
	m_ecsEngine.TagEntity("Camera", cameraID);

	m_ecsEngine.AddComponent<TransformComponent>(cameraID)->InitializeTransformComponent(Vector2f(0.0f, 0.0f));
	m_ecsEngine.AddComponent<MovementComponent>(cameraID)->InitializeMovementComponent(Vector2f(0.0f, 0.0f), 600);
	m_ecsEngine.AddComponent<ScriptComponent>(cameraID)->InitializeScriptComponent(ScriptAllocator.GetScript<CameraScript>());
}

void Scene::RefreshReloadedScene()
{
	if (this->m_sceneClosed)
	{
		if (!Mouse.leftButtonDown)
		{
			this->m_sceneClosed = false;
		}
	}
}

void Scene::ControlCamera(const float& deltaTime)
{
	//get camera movement component
	std::shared_ptr<TransformComponent> cameraTransformComponent{ m_ecsEngine.GetComponent<TransformComponent>(m_ecsEngine.GetTaggedEntities("Camera")[0])};
	
	if (m_editorMode)
	{
		static const HAPI_TMouseData& mouseData{ HAPI.GetMouseData() };

		if (mouseData.rightButtonDown)
		{
			Vector2i moveToMouse{ Vector2i(mouseData.x, mouseData.y) - m_previousMousePosition };
			cameraTransformComponent->position = (cameraTransformComponent->position.ToType<int>() += moveToMouse.Invert()).ToType<float>();
		}

		m_previousMousePosition.x = mouseData.x;
		m_previousMousePosition.y = mouseData.y;
	}
}

Vector2i Scene::GetCameraOffset()
{
	//get camera movement component
	std::shared_ptr<TransformComponent> cameraTransformComponent{ m_ecsEngine.GetComponent<TransformComponent>(m_ecsEngine.GetTaggedEntities("Camera")[0]) };

	return cameraTransformComponent->position.Invert().ToType<int>();
}

void Scene::BaseUpdate(const float& deltaTime, SceneManager& sceneManager)
{
	RefreshReloadedScene();
	if (!this->m_sceneClosed)
	{
		if (Keyboard.scanCode[HK_ESCAPE])
		{
			HAPI.Close();
			return;
		}
		Update(deltaTime, sceneManager);
		ProcessUI(sceneManager);
	}
}
