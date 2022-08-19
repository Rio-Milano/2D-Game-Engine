#ifndef WIDGET_H
#define WIDGET_H

class ECS_Engine;
class RenderManager;
class UI_Manager;

#include<memory>
#include<string>

class Widget
{
public:
	Widget() = default;
	~Widget() = default;

	const uint32_t& GetWidgetID()const { return m_entityID; };

	/*
	every widget needs to know how to create itself using the ECS to hold its data
	and the render manager to queue itself to be rendered
	*/
	virtual void CreateWidget(const std::string& widgetName, RenderManager& renderManager, UI_Manager& uiManager, ECS_Engine& ecs) = 0;


	virtual void HideWidget(ECS_Engine& ecs, RenderManager& renderManager);
	virtual void ShowWidget(ECS_Engine& ecs, RenderManager& renderManager);

	/*
	Some widgets are purely bassed on state infomation and when we load another scene then reverse the process the state should reset. I.e. clicking on a button state should be reset to idle state
	*/
	virtual void ResetWidget(ECS_Engine& ecs) {};

protected:
	uint32_t m_entityID{};
};

#endif

/*
#include"../ECS/Base/ECS_Engine.h"
#include"../ECS/Base/Core_ECS_Assets.h"
#include"../Managers/AllManagerHeadders.h"

void Widget::HideWidget(ECS_Engine& ecs, RenderManager& renderManager)
{
	std::shared_ptr<RenderComponent> renderComponent{ ecs.GetComponent<RenderComponent>(this->m_entityID) };
	renderManager.RemoveEntityIdFromRenderLayer(this->m_entityID, renderComponent->m_renderLayer);
}

void Widget::ShowWidget(ECS_Engine& ecs, RenderManager& renderManager)
{
	std::shared_ptr<RenderComponent> renderComponent{ ecs.GetComponent<RenderComponent>(this->m_entityID) };
	renderManager.AddEntityIdToRenderLayer(this->m_entityID, renderComponent->m_renderLayer);
}

*/