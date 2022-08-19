#include "ListBoxWidgetSlideBarScript.h"

#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"
#include"../UI/UI Core/UI_Manager.h"
#include"../UI/UI Widget/WidgetHeadders.h"
#include"../ECS/Base/ScriptAllocator.h"
#define ScriptAllocator ScriptAllocator::GetInstance()

void ListBoxWidgetSlideBarScript::FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID)
{
	std::shared_ptr<EventComponent> listBoxEventComponent{ecs.GetComponent<EventComponent>(entityID)};
	std::shared_ptr<ListBoxWidget> listBoxWidget{ uiManager.GetWidget<ListBoxWidget>(this->m_listBoxName) };
	std::shared_ptr<TransformComponent> listBoxTransform{ecs.GetComponent<TransformComponent>(entityID)};

	//rescale list box height bassed on amout of text elements
	if (listBoxWidget->m_listOfWidgets.size() < this->m_defaultMaxNumOfElementsShownAtOnce)
		listBoxEventComponent->m_eventRect.y2 = listBoxEventComponent->m_eventRect.y1 + static_cast<int>(listBoxWidget->m_listOfWidgets.size()) * listBoxWidget->m_defaultTextElementHeight;

	Vector2i mousePosition{Mouse.x, Mouse.y};


	if (UI_Manager::GetEntityEngaged() == entityID)
	{
		//check for scrolling
		int mouseWheelMovement{ Mouse.wheelMovement };

		if (mouseWheelMovement)
		{
			if (mouseWheelMovement < 0)
				listBoxWidget->ScrollWidgets(ecs, -m_slideBarSpeed);
			else
				listBoxWidget->ScrollWidgets(ecs, m_slideBarSpeed);
		}
	

		//if the ui manager is not engaged and the mouse is holding left click on the list box rect
		if (listBoxEventComponent->m_isMouseLeftClick)
		{
			listBoxEventComponent->m_tagComponent = true;
			UI_Manager::SetEntityEngaged(static_cast<int32_t>(entityID));
		}
		if(!Mouse.leftButtonDown)
			listBoxEventComponent->m_tagComponent = false;

	//if the list box is tagged
	if (listBoxEventComponent->m_tagComponent)
	{
		//if no click point has been set
		if (!m_hasClickPointOffsetBeenSet)
		{
			//set click point to mouse pos
			m_clickPointOffset = mousePosition;
			//mark flag
			m_hasClickPointOffsetBeenSet = true;
		}
		//calculate mouse move offset from prev pos
		Vector2i movementByMouse{ mousePosition - m_clickPointOffset };
		//update prev pos
		m_clickPointOffset = mousePosition;
		//update widget position
		listBoxWidget->TranslateWidget(listBoxTransform->position + movementByMouse.ToType<float>(), ecs);
	}
	//if the left mouse button is not in use
	else
		//make the click point offset flag false for next engagement
		m_hasClickPointOffsetBeenSet = false;
	}
}

void ListBoxWidgetSlideBarScript::ReInitialize()
{
	m_slideBarSpeed = 1.0f;
}

std::shared_ptr<BaseScript> ListBoxWidgetSlideBarScript::CloneScript()
{
	std::shared_ptr<ListBoxWidgetSlideBarScript> newScript{ScriptAllocator.GetScript<ListBoxWidgetSlideBarScript>()};
	newScript->InitializeScript(m_listBoxName, m_slideBarSpeed);
	return newScript;
}
