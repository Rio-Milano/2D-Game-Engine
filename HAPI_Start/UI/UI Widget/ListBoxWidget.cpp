#include "ListBoxWidget.h"

#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../UI Widget/WidgetHeadders.h"
#include"../Managers/AllManagerHeadders.h"

#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()

void ListBoxWidget::CreateWidget(const std::string& widgetName, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef)
{
	m_entityID = ecsEngineRef.InstantiateEntity();
	//we only need a referance point to render the list box
	ecsEngineRef.AddComponent<TransformComponent>(m_entityID)->InitializeTransformComponent(Vector2f(0.0f, 0.0f));
	/*
	the event component holds a rect instanceand this represents the size of the list box,
	by leaving x1, y1 to 0 and using x2 and y2 as the size we then translate the rect using the transform component to the screen position
	as its not a world render it will not move in world space only screen space
	*/
	
	ecsEngineRef.AddComponent<EventComponent>(m_entityID)->InitializeEventComponent(IntRect(0, 0, 50, 50));
	ecsEngineRef.AddComponent<RenderComponent>(m_entityID)->InitializeRenderComponent
		(
			RenderComponent::RenderConfigs::DrawBoundingBox |
			RenderComponent::RenderConfigs::DrawBoundBoxEventComponent,

			RenderComponent::RenderLayer::UI,

			true,

			false

			);
	std::shared_ptr< ListBoxWidgetSlideBarScript> script{ ScriptAllocator.GetScript<ListBoxWidgetSlideBarScript>() };
	script->InitializeScript(widgetName, 20.0f);
	ecsEngineRef.AddComponent<ScriptComponent>(m_entityID)->InitializeScriptComponent(script);
	renderManagerRef.AddEntityIdToRenderLayer(ecsEngineRef, m_entityID, RenderComponent::RenderLayer::UI);
}

void ListBoxWidget::HideWidget(ECS_Engine& ecs, RenderManager& renderManager)
{
	renderManager.RemoveEntityIdFromRenderLayer(this->m_entityID, ecs.GetComponent<RenderComponent>(this->m_entityID)->m_renderLayer);

	for (const uint32_t& entityID : this->m_listOfWidgets)
		renderManager.RemoveEntityIdFromRenderLayer(entityID, ecs.GetComponent<RenderComponent>(entityID)->m_renderLayer);
}

void ListBoxWidget::ShowWidget(ECS_Engine& ecs, RenderManager& renderManager)
{
	renderManager.AddEntityIdToRenderLayer(ecs, this->m_entityID, ecs.GetComponent<RenderComponent>(this->m_entityID)->m_renderLayer);

	for (const uint32_t& entityID : this->m_listOfWidgets)
		renderManager.AddEntityIdToRenderLayer(ecs, entityID, ecs.GetComponent<RenderComponent>(entityID)->m_renderLayer);
}


void ListBoxWidget::TranslateWidget(const Vector2f& translateTo, ECS_Engine& ecsEngineRef) const
{
	std::shared_ptr<TransformComponent> transformComponent{ ecsEngineRef.GetComponent<TransformComponent>(this->GetWidgetID()) };

	//get a translation vector to the translateTo point
	Vector2f translateBy{translateTo - transformComponent->position};

	//move the list box to the translation point
	transformComponent->position = transformComponent->position + translateBy;

	for (const uint32_t& textElementID : this->m_listOfWidgets)
	{
		transformComponent = ecsEngineRef.GetComponent<TransformComponent>(textElementID);
		transformComponent->position = transformComponent->position + translateBy;
	}
}

void ListBoxWidget::ScrollWidgets(ECS_Engine& ecsEngineRef, const float& moveWidgetsByY)
{
	if (moveWidgetsByY == 0)
		return;

	//if the list box has widgets
	if (this->m_listOfWidgets.size() > 0)
	{
		//get the first text widget in the list
		std::shared_ptr<TransformComponent> transformComponent{ nullptr };
		
		//if the top text element is trying to be scrolled beyond top of event rect of list box
		if (ecsEngineRef.GetComponent<TransformComponent>(this->m_listOfWidgets.front())->position.y + moveWidgetsByY > ecsEngineRef.GetComponent<TransformComponent>(this->GetWidgetID())->position.y)
			return;

		//if the bottom text element is trying to be scrolled beyond the top of the event rect of the list box
		if (
			ecsEngineRef.GetComponent<TransformComponent>(this->m_listOfWidgets.back())->position.y 
			+ this->m_defaultTextElementHeight 
			+ moveWidgetsByY 
			
			< 
			
			ecsEngineRef.GetComponent<TransformComponent>(this->GetWidgetID())->position.y 
			+ ecsEngineRef.GetComponent<EventComponent>(this->GetWidgetID())->m_eventRect.GetRectHeight()
			)
			return;



		for (const uint32_t& textElementID : this->m_listOfWidgets)
		{
			transformComponent = ecsEngineRef.GetComponent<TransformComponent>(textElementID);
			transformComponent->position.y += moveWidgetsByY;
		}
	}
}

uint32_t ListBoxWidget::UniformWidgetConfig(const std::string& widgetName, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef)
{
	/*
	Parent data
	*/
	std::shared_ptr<TransformComponent> listBoxTransform{ ecsEngineRef.GetComponent<TransformComponent>(this->GetWidgetID()) };
	std::shared_ptr<EventComponent>	listBoxWidgetEvent{ ecsEngineRef.GetComponent<EventComponent>(this->GetWidgetID()) };
	
	/*
	Create and Init text widget
	*/
	std::shared_ptr<TextWidget> textWidget{ std::make_shared<TextWidget>() };
	textWidget->CreateWidget(widgetName, renderManagerRef, uiManager, ecsEngineRef);

	//get text widget entitiyID
	const uint32_t& textWidgetID{ textWidget->GetWidgetID() };

	/*
	Widget Data
	*/
	std::shared_ptr<TransformComponent> textWidgetTransform{ ecsEngineRef.GetComponent<TransformComponent>(textWidgetID) };
	std::shared_ptr<EventComponent>	textWidgetEvent{ ecsEngineRef.GetComponent<EventComponent>(textWidgetID) };
	std::shared_ptr<TextComponent> textWidgetTextComponent{ ecsEngineRef.GetComponent<TextComponent>(textWidgetID) };
	std::shared_ptr<RenderComponent> textWidgetRenderComponent{ ecsEngineRef.GetComponent<RenderComponent>(textWidgetID) };


	/*
	configure the text widget
	*/

	//configure the transform component
	textWidgetTransform->position = listBoxTransform->position;

	//resize the text widget width
	textWidgetEvent->m_eventRect.x2 = listBoxWidgetEvent->m_eventRect.x2;


	/*
	Make sure that the widget appears underneath the widget closest to the end
	*/
	if (this->m_listOfWidgets.size() > 0)
	{
		std::shared_ptr<EventComponent> lastTextWidget{ ecsEngineRef.GetComponent<EventComponent>(this->m_listOfWidgets.back()) };
		textWidgetTransform->position.y = static_cast<float>(lastTextWidget->m_eventRect.y2);
	}

	textWidgetEvent->m_eventRect.y2 = textWidgetEvent->m_eventRect.y1 + this->m_defaultTextElementHeight;

	textWidgetEvent->m_eventRect.Translate(textWidgetTransform->position.ToType<int>());

	this->m_listOfWidgets.emplace_back(textWidgetID);

	//tell render component to clip to this event rect
	textWidgetRenderComponent->m_renderConfigs |= RenderComponent::RenderConfigs::ClipTextToEventRect;

	//tell text component which entity to use to retrieve event component for text clipping
	textWidgetTextComponent->clipToEntityEventRectID = this->GetWidgetID();

	/*
	Set the event parent
	*/
	textWidgetEvent->parentID = m_entityID;

	return textWidgetID;
}


uint32_t ListBoxWidget::AddHeadding(const std::string& headdingNameArgRef, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef)
{
	 uint32_t textWidgetID{ UniformWidgetConfig(headdingNameArgRef, renderManagerRef, uiManager, ecsEngineRef) };

	std::shared_ptr<TextComponent> textWidgetTextComponent{ ecsEngineRef.GetComponent<TextComponent>(textWidgetID) };

	//configure the text
	textWidgetTextComponent->m_fillColour = this->m_headdingColor;
	textWidgetTextComponent->m_styleFlags = this->m_headdingTextStyle;
	textWidgetTextComponent->m_textSize = this->m_headdingTextSize;
	textWidgetTextComponent->m_text = this->m_headdingOffset + headdingNameArgRef;

	return textWidgetID;
}

uint32_t ListBoxWidget::AddSubHeadding(const std::string& subHeaddingNameArgRef, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef)
{
	uint32_t textWidgetID{ UniformWidgetConfig(subHeaddingNameArgRef, renderManagerRef, uiManager, ecsEngineRef) };

	std::shared_ptr<TextComponent> textWidgetTextComponent{ ecsEngineRef.GetComponent<TextComponent>(textWidgetID) };

	//configure the text
	textWidgetTextComponent->m_fillColour = this->m_subHeaddingColor;
	textWidgetTextComponent->m_styleFlags = this->m_subHeaddingTextStyle;
	textWidgetTextComponent->m_textSize = this->m_subHeaddingTextSize;
	textWidgetTextComponent->m_text = this->m_subHeaddingOffset + subHeaddingNameArgRef;

	return textWidgetID;
}



uint32_t ListBoxWidget::AddTextElement(const std::string& textElementNameArgRef, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef)
{
	uint32_t textWidgetID{ UniformWidgetConfig(textElementNameArgRef, renderManagerRef, uiManager, ecsEngineRef) };

	std::shared_ptr<TextComponent> textWidgetTextComponent{ ecsEngineRef.GetComponent<TextComponent>(textWidgetID) };

	std::shared_ptr<TextElementInteractionResponseScript> newScript{ScriptAllocator.GetScript<TextElementInteractionResponseScript>()};
	newScript->InitializeScript(HAPI_TColour::HORRID_PINK, HAPI_TColour::GREEN, HAPI_TColour::YELLOW, HAPI_TColour::WHITE);
	ecsEngineRef.AddComponent<ScriptComponent>(textWidgetID)->InitializeScriptComponent(newScript);

	//configure the text
	textWidgetTextComponent->m_fillColour = this->m_textElementColor;
	textWidgetTextComponent->m_styleFlags = this->m_textElementTextStyle;
	textWidgetTextComponent->m_textSize = this->m_textElementTextSize;
	textWidgetTextComponent->m_text = this->m_textElementOffset + textElementNameArgRef;

	return textWidgetID;


}

