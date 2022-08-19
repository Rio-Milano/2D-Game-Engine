#include "TextElementInteractionResponseScript.h"

#include"../ECS/Base/Core_ECS_Assets.h"
#include"../ECS/Base/ECS_Engine.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()

int32_t TextElementInteractionResponseScript::currentTaggedEntity{ -1 };


void TextElementInteractionResponseScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	//get entity components
	std::shared_ptr<EventComponent> eventComponent{ ecs.GetComponent<EventComponent>(entityID) };
	std::shared_ptr<TextComponent> textComponent{ ecs.GetComponent<TextComponent>(entityID) };

	//if the mouse is hovering over the event rect
	if (eventComponent->m_isMouseOver)
		textComponent->m_fillColour = this->m_onHoverColor;


	//if the mouse is not hovering over the event rect
	else
	{
		//if the event component is not tagged
		if (!eventComponent->m_tagComponent)
			//set the text color to idle 
			textComponent->m_fillColour = m_idleColor;
	}

	//if the component is tagged
	if (eventComponent->m_tagComponent)
		//set color to tagged color
		textComponent->m_fillColour = this->m_tagComponentColor;

	//if the event component is registering a left click and diddnt register a left click in the previous update
	if (eventComponent->m_isMouseLeftClick && !eventComponent->m_mouseLeftClickHolding)
	{
		//if re clicking on the tagged entity
		if (currentTaggedEntity == static_cast<int32_t>(entityID))
		{
			//unbind the current tagged entity
			currentTaggedEntity = -1;
			//make the event component no longer tagged
			eventComponent->m_tagComponent = false;

		}
		//if clicking on a different entity
		else
		{
			//if tagged entity has already been set
			if (currentTaggedEntity != -1)
				//unbind current tagged entity
				ecs.GetComponent<EventComponent>(static_cast<uint32_t>(currentTaggedEntity))->m_tagComponent = false;

			//make current entity context
			currentTaggedEntity = static_cast<int32_t>(entityID);
			//make current entity tagged
			eventComponent->m_tagComponent = true;
		}
	}
}

void TextElementInteractionResponseScript::ReInitialize()
{

}


std::shared_ptr<BaseScript> TextElementInteractionResponseScript::CloneScript()
{
	std::shared_ptr<TextElementInteractionResponseScript> newScript{ScriptAllocator.GetScript<TextElementInteractionResponseScript>()};
	newScript->InitializeScript(m_idleColor, m_onHoverColor, m_onClickColor, m_tagComponentColor);
	return newScript;
}