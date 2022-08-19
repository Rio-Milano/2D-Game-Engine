#include"EditorSystem.h"

#include"../Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../Scenes/SceneCore/Scene.h"

#include<string>
#include<iostream>

//private system functions


EditorSystem::EditorSystem(const UpdatePriority& updatePriority)
	:System(updatePriority)
{
	m_componentEngine.AllocateComponents<EditorComponent>();
}

std::shared_ptr<BaseComponent> EditorSystem::DuplicateComponent(const uint32_t& entityID, const uint32_t& forignEntityID, ECS_Engine& forignECS)
{
	//we may not have a editor component
	std::shared_ptr<EditorComponent> toClone{ std::static_pointer_cast<EditorComponent>(m_componentEngine.GetComponent(entityID)) };
	if (!toClone)
	{
		std::cerr << std::string("Editor Component of entity :" + std::to_string(entityID) + " was not found\n");
		return nullptr;
	}

	std::shared_ptr<EditorComponent> duplicatedComponent = forignECS.AddComponent<EditorComponent>(forignEntityID);

	duplicatedComponent->InitializeEditorComponent(
		toClone->m_enableMoveByMouse, toClone->m_spatialLock
	);

	duplicatedComponent->m_oneInstanceOnly = toClone->m_oneInstanceOnly;
	return duplicatedComponent;
}

void EditorSystem::SystemDisableProcedure(ECS_Engine& ecs)
{
	if (this->m_backgroundFocus != -1)
	{
		std::shared_ptr<RenderComponent> backgroundFocusRenderComponent{ ecs.GetComponent<RenderComponent>(this->m_backgroundFocus) };
		if(backgroundFocusRenderComponent != nullptr)
			backgroundFocusRenderComponent->m_renderConfigs ^= (RenderComponent::RenderConfigs::DrawBoundBoxEventComponent | RenderComponent::RenderConfigs::DrawBoundingBox);
		this->m_backgroundFocus = -1;
	}
}



void EditorSystem::UpdateComponent(const uint32_t& entityID, ECS_Engine& ecs,  ECS_Engine& forignEcs, UI_Manager& uiManager, RenderManager& renderManager, const float& deltaTime)
{
	std::shared_ptr<EditorComponent> editorComponent{ ecs.GetComponent<EditorComponent>(entityID) };


	if (!Mouse.leftButtonDown && this->m_clickFocusOnEntity == entityID || (this->m_clickFocusOnEntity != -1 && UI_Manager::GetEntityEngaged() != this->m_clickFocusOnEntity))
	{
		editorComponent->m_hasClickPointOffsetBeenSet = false;
		editorComponent->m_focus = false;

		this->m_backgroundFocus = this->m_clickFocusOnEntity;
		this->m_clickFocusOnEntity = -1;
	}

	

	if (!Scene::m_editorMode)
		this->m_backgroundFocus = -1;

	if (Keyboard.scanCode[HK_F3])
		this->m_backgroundFocus = -1;

	std::shared_ptr<EventComponent> eventComponent{ ecs.GetComponent<EventComponent>(entityID) };
	assert(eventComponent != nullptr);

	std::shared_ptr<RenderComponent> renderComponent{ ecs.GetComponent<RenderComponent>(entityID) };
	assert(renderComponent != nullptr);


	if (renderComponent->m_renderConfigs & RenderComponent::RenderConfigs::DrawBoundingBox)
		renderComponent->m_renderConfigs ^= RenderComponent::RenderConfigs::DrawBoundBoxEventComponent | RenderComponent::RenderConfigs::DrawBoundingBox;


	if (this->m_clickFocusOnEntity == entityID || this->m_backgroundFocus == entityID)
	{
		renderComponent->m_renderConfigs |= RenderComponent::RenderConfigs::DrawBoundBoxEventComponent | RenderComponent::RenderConfigs::DrawBoundingBox;
		if (this->m_clickFocusOnEntity == entityID)
			eventComponent->m_eventRect.m_debugRectColor = HAPI_TColour::GREEN;
		else if (this->m_backgroundFocus == entityID)
			eventComponent->m_eventRect.m_debugRectColor = HAPI_TColour::RED;
	}



	if (editorComponent->m_enableMoveByMouse)
	{
		
		std::shared_ptr<TransformComponent> cameraTransform{ ecs.GetComponent<TransformComponent>(ecs.GetTaggedEntities("Camera")[0]) };
		assert(cameraTransform != nullptr);


		Vector2i currentMousePos(Mouse.x + static_cast<int>(cameraTransform->position.x), Mouse.y + static_cast<int>(cameraTransform->position.y));



		if (
			this->m_clickFocusOnEntity == -1 &&
			eventComponent->m_isMouseLeftClick &&
			!eventComponent->m_mouseLeftClickHolding &&
			UI_Manager::GetEntityEngaged() == entityID
			)
		{
			editorComponent->m_focus = true;
			this->m_clickFocusOnEntity = static_cast<int32_t>(entityID);
			this->m_backgroundFocus = -1;
		}
		static Vector2i previousMousePosition = currentMousePos;

		if (editorComponent->m_focus)
		{
			std::shared_ptr<TransformComponent> transformComponent{ ecs.GetComponent<TransformComponent>(entityID) };
			if (!transformComponent)
			{
				std::cerr << std::string("Transform Component for entity :" + std::to_string(entityID) + " was not found!");
				return;
			}

			if (!editorComponent->m_hasClickPointOffsetBeenSet)
			{
				editorComponent->m_clickPointOffset = currentMousePos;
				editorComponent->m_hasClickPointOffsetBeenSet = true;
			}
			
			if (editorComponent->m_spatialLock)
			{
				Vector2i tileSize{ 64, 64 };

				//with snapping

				Vector2i mouseMoved = currentMousePos - editorComponent->m_clickPointOffset;

				int
					xTilesAccross{ mouseMoved.x / tileSize.x },
					yTilesAccross{ mouseMoved.y / tileSize.y };

				editorComponent->m_clickPointOffset.x += xTilesAccross * tileSize.x;
				editorComponent->m_clickPointOffset.y += yTilesAccross * tileSize.y;

				transformComponent->position.x += xTilesAccross * tileSize.x;
				transformComponent->position.y += yTilesAccross * tileSize.y;

				transformComponent->position.x = (float)(((int)transformComponent->position.x / (int)tileSize.x) * tileSize.x);
				transformComponent->position.y = (float)(((int)transformComponent->position.y / (int)tileSize.y) * tileSize.y);
			}
			else
			{

				
				//Without snapping
				Vector2i clickPointOffsetFromOffset = currentMousePos - editorComponent->m_clickPointOffset;

				transformComponent->position = transformComponent->position + clickPointOffsetFromOffset.ToType<float>();
				editorComponent->m_clickPointOffset = currentMousePos;
				
			}
		}
	}


	//if deleting an entity
	if (Keyboard.scanCode[HK_DELETE])
	{
		//if entity has been selected in background
		if (this->m_backgroundFocus == entityID)
		{
			//mark entity to be deleted
			ecs.DeleteEntity(entityID);

			//unbind focus from background
			this->m_backgroundFocus = -1;
		}
	}

}

void EditorSystem::ResetComponent(const uint32_t& entityID, ECS_Engine& ecs)
{
	//we may not have a editor component
	std::shared_ptr<EditorComponent> editorComponent{ std::static_pointer_cast<EditorComponent>(m_componentEngine.GetComponent(entityID)) };
	if (!editorComponent)
	{
		std::cerr << std::string("Editor Component of entity :" + std::to_string(entityID) + " was not found\n");
		return;
	}

	editorComponent->m_enableMoveByMouse = true;
}