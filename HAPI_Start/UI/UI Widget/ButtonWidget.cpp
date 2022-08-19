#include "ButtonWidget.h"

#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()


#include<HAPI_lib.h>
using namespace HAPISPACE;

void ButtonWidget::CreateWidget(const std::string& widgetName, RenderManager& renderManager, UI_Manager& uiManager, ECS_Engine& ecs)
{
	//create an id for the widget
	m_entityID =  ecs.InstantiateEntity();

	ecs.AddComponent<TransformComponent>(m_entityID)->InitializeTransformComponent(Vector2f(0.0f, 0.0f));
	ecs.AddComponent<SpriteComponent>(m_entityID)->m_sprite.InitializeSprite(nullptr, false, Effects::NoEffects);
	ecs.AddComponent<TextComponent>(m_entityID)->InitializeTextComponent(HAPI_TColour::WHITE, "<INSERT TEXT>");
	ecs.AddComponent<RenderComponent>(m_entityID)->InitializeRenderComponent(RenderComponent::RenderConfigs::Sprite | RenderComponent::RenderConfigs::Text, RenderComponent::RenderLayer::UI, true, false);
	ecs.AddComponent<EventComponent>(m_entityID)->InitializeEventComponent(IntRect(0, 0, 0, 0));
	ecs.AddComponent<ScriptComponent>(m_entityID)->InitializeScriptComponent(ScriptAllocator.GetScript<ButtonControllerScript>());


	renderManager.AddEntityIdToRenderLayer(ecs, m_entityID, RenderComponent::RenderLayer::UI);
	


}

void ButtonWidget::ResetWidget(ECS_Engine& ecs)
{
	std::shared_ptr<ButtonControllerScript> script{ std::static_pointer_cast<ButtonControllerScript>(ecs.GetComponent<ScriptComponent>(this->m_entityID)->m_script) };

	//reset text color of start button
	ecs.GetComponent<TextComponent>(this->m_entityID)->m_fillColour = script->m_idleStateTextColour;

	std::shared_ptr<SpriteComponent> spriteComponent{ ecs.GetComponent<SpriteComponent>(this->m_entityID) };

	//reset texture color of start button
	TextureManager.FillBufferWithColor
	(
		spriteComponent->m_sprite.m_textureRef->m_textureBuffer,
		spriteComponent->m_sprite.m_textureRef->m_textureSize,
		script->m_idleStateButtonColour
	);

	std::shared_ptr<EventComponent> eventComponent{ecs.GetComponent<EventComponent>(m_entityID)};
	eventComponent->m_isMouseOver = false;
	eventComponent->m_isMouseLeftClick = false;
	eventComponent->m_mouseLeftClickHolding = false;

}

void ButtonWidget::InitializeButton
(
	ECS_Engine& ecs,
	const Vector2f& position,
	const Vector2i& buttonSize,
	const HAPI_TColour& buttonFillColor,
	const HAPI_TColour& buttonTextColour,
	const std::string& buttonText,
	const Vector2f textOffset,
	const unsigned int textSize
)
{
	uint32_t thisID{this->GetWidgetID()};

	//create an empty texture for the button sprite component
	BYTE* emptyButtonTexture{ nullptr };

	//remove bytes per pixel notation

	/*
	Create the texture for the button
	*/

	//fill the texture buffer with needed memory
	TextureManager.FillNullTextureBuffer(&emptyButtonTexture, buttonSize.ToType<int>(), 4);
	//set the pixels to be set color
	TextureManager.FillBufferWithColor(emptyButtonTexture, buttonSize.ToType<int>(), buttonFillColor);
	//create a texture object
	std::shared_ptr<Texture> texture{ TextureManager.MakeTexture(emptyButtonTexture, buttonSize.ToType<int>()) };


	std::shared_ptr<TransformComponent> transform{ ecs.GetComponent<TransformComponent>(thisID) };
	transform->position = position;

	ecs.GetComponent<SpriteComponent>(thisID)->m_sprite.m_textureRef = texture;

	ecs.GetComponent<TextComponent>(thisID)->m_fillColour = buttonTextColour;
	ecs.GetComponent<TextComponent>(thisID)->m_text = buttonText;
	ecs.GetComponent<TextComponent>(thisID)->m_offsetFromTransform = textOffset;
	ecs.GetComponent<TextComponent>(thisID)->m_textSize = textSize;

	std::shared_ptr<EventComponent> eventComponent{ ecs.GetComponent<EventComponent>(thisID) };
	eventComponent->m_eventRect.x1 = (int)transform->position.x;
	eventComponent->m_eventRect.y1 = (int)transform->position.y;
	eventComponent->m_eventRect.x2 = (int)transform->position.x + buttonSize.x;
	eventComponent->m_eventRect.y2 = (int)transform->position.y + buttonSize.y;



}

bool ButtonWidget::OneStepClick(ECS_Engine& ecs)
{
	std::shared_ptr<EventComponent> eventComponent{ ecs.GetComponent<EventComponent>(this->m_entityID) };
	return (eventComponent->m_isMouseLeftClick && !eventComponent->m_mouseLeftClickHolding);
}
