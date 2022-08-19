#include "TextWidget.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"

#include<HAPI_lib.h>
using namespace HAPISPACE;

void TextWidget::CreateWidget(const std::string& widgetName, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef)
{
	//create an id for the text widget
	m_entityID = ecsEngineRef.InstantiateEntity();

	/*
	* Attach base components for the entity
	* Set all component data to the default
	*/
	ecsEngineRef.AddComponent<MovementComponent>(m_entityID)->InitializeMovementComponent(Vector2f(0, 0));
	ecsEngineRef.AddComponent<TransformComponent>(m_entityID)->InitializeTransformComponent(Vector2f(0, 0));
	ecsEngineRef.AddComponent<TextComponent>(m_entityID)->InitializeTextComponent(HAPI_TColour::WHITE, "<Insert Text>");
	ecsEngineRef.AddComponent<RenderComponent>(m_entityID)->InitializeRenderComponent
			(RenderComponent::RenderConfigs::Text,			
			RenderComponent::RenderLayer::UI, true, false);

	ecsEngineRef.AddComponent<EventComponent>(m_entityID)->InitializeEventComponent(IntRect(0, 0, 50, 50));
	renderManagerRef.AddEntityIdToRenderLayer(ecsEngineRef, m_entityID, RenderComponent::RenderLayer::UI);
}

