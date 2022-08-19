#include "ButtonControllerScript.h"

#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()

void ButtonControllerScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	if (m_useStateSettingColours)
	{
		std::shared_ptr<EventComponent> eventComponent{ ecs.GetComponent<EventComponent>(entityID) };
		if (eventComponent == nullptr)
		{
			std::cerr << "Button does not have event component!\n";
			return;
		}

		std::shared_ptr<SpriteComponent> spriteComponent{ ecs.GetComponent<SpriteComponent>(entityID) };
		if (spriteComponent == nullptr)
		{
			std::cerr << "Button does not have sprite component!\n";
			return;
		}

		std::shared_ptr<TextComponent> textComponent{ ecs.GetComponent<TextComponent>(entityID) };
		if (textComponent == nullptr)
		{
			std::cerr << "Button does not have text component!\n";
			return;
		}

		//if mouse not over button then button is in idle state
		if (!eventComponent->m_isMouseOver)
		{
			textComponent->m_fillColour = this->m_idleStateTextColour;
			TextureManager.FillBufferWithColor(spriteComponent->m_sprite.m_textureRef->m_textureBuffer, spriteComponent->m_sprite.m_textureRef->m_textureSize, this->m_idleStateButtonColour);
		}

		//if mouse over button but not clicking then button is in hover state
		else if (eventComponent->m_isMouseOver && !eventComponent->m_isMouseLeftClick)
		{
			textComponent->m_fillColour = this->m_hoverStateTextColour;
			TextureManager.FillBufferWithColor(spriteComponent->m_sprite.m_textureRef->m_textureBuffer, spriteComponent->m_sprite.m_textureRef->m_textureSize, this->m_hoverStateButtonColour);
		}

		//if mouse over button and clicking then buttin is in clicking state
		else if (eventComponent->m_isMouseOver && eventComponent->m_isMouseLeftClick)
		{
			textComponent->m_fillColour = this->m_clickingStateTextColour;
			TextureManager.FillBufferWithColor(spriteComponent->m_sprite.m_textureRef->m_textureBuffer, spriteComponent->m_sprite.m_textureRef->m_textureSize, this->m_clickingStateButtonColour);
		}
	}
}

void ButtonControllerScript::ReInitialize()
{

}

std::shared_ptr<BaseScript> ButtonControllerScript::CloneScript()
{
	std::shared_ptr<ButtonControllerScript> newScript{ ScriptAllocator.GetScript<ButtonControllerScript>() };
	return newScript;
}
