#ifndef UI_MANAGER_H
#define UI_MANAGER_H


#include"UI_CoreHeadders.h"

#include<unordered_map>
#include<string>
#include<memory>
#include<iostream>

#include"../Managers/RenderManager.h"
#include"../ECS/Base/ECS_Engine.h"

class UI_Manager
{
public:
	UI_Manager(RenderManager& renderManagerRefArg, ECS_Engine& ecsEngineRefArg);
	~UI_Manager() = default;

	template<class T>
	std::shared_ptr<T> GetWidget(const std::string& widgetName);

	void AddWidget(const std::string& widgetName, const std::shared_ptr<Widget>& widget);

	const static int32_t& GetEntityEngaged() { return UI_EngagedTo_EntityID; };
	static void SetEntityEngaged(const int32_t& entity) { UI_EngagedTo_EntityID = entity; };

	void Clear() { m_UI_WidgetMap.clear(); }
private:
	static int32_t UI_EngagedTo_EntityID;

	std::unordered_map<std::string, std::shared_ptr<Widget>> m_UI_WidgetMap{};
	
	RenderManager& m_renderManagerRef;
	ECS_Engine& m_ecsEngineRef;
};


template<class T>
inline std::shared_ptr<T> UI_Manager::GetWidget(const std::string& widgetName)
{
	//if widget dosen't exist
	if (m_UI_WidgetMap.find(widgetName) == m_UI_WidgetMap.end())
	{
		std::cerr << std::string("Widget :" + widgetName + " was not found!\n");
		return nullptr;
	}
	return std::static_pointer_cast<T>(m_UI_WidgetMap[widgetName]);
}

#endif