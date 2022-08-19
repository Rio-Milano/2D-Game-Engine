#include "UI_Manager.h"

int32_t UI_Manager::UI_EngagedTo_EntityID{-1};

UI_Manager::UI_Manager(RenderManager& renderManagerRefArg, ECS_Engine& ecsEngineRefArg)
	:
	m_ecsEngineRef(ecsEngineRefArg), 
	m_renderManagerRef(renderManagerRefArg)
{
}

void UI_Manager::AddWidget(const std::string& widgetName, const std::shared_ptr<Widget>& widget)
{
	//if the widget exists
	if (m_UI_WidgetMap.find(widgetName) != m_UI_WidgetMap.end())
	{
		std::cerr << std::string("Widget :" + widgetName + " already exists!\n");
		return;
	}
	widget->CreateWidget(widgetName, m_renderManagerRef, *this, m_ecsEngineRef);
	m_UI_WidgetMap[widgetName] = widget;
}
