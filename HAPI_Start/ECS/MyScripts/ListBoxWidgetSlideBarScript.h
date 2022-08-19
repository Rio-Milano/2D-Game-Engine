#ifndef LISTBOXWIDGETSLIDEBARSCRIPT_H
#define LISTBOXWIDGETSLIDEBARSCRIPT_H

#include"../ECS/Base/BasesScript.h"
#include"../DataStructures/Vector.h"

class UI_Manager;

#include<string>

class ListBoxWidgetSlideBarScript final : public BaseScript
{
public:

	void InitializeScript(const std::string& listBoxName, const float& slideBarSpeedArg)
	{
		m_listBoxName = listBoxName;
		m_slideBarSpeed = slideBarSpeedArg;
	};

	void FixedUpdate(ECS_Engine& ecs, ECS_Engine& forignECS, UI_Manager& uiManager, RenderManager& renderManager, const uint32_t& entityID) override;

	void ReInitialize() override;

	std::shared_ptr<BaseScript> CloneScript() override;

private:
	float m_slideBarSpeed{1.0f};
	std::string m_listBoxName{""};

	int m_defaultMaxNumOfElementsShownAtOnce{ 16 };

	/*
	Not using editor component as its not a game entity
	*/
	bool m_hasClickPointOffsetBeenSet{ false };
	Vector2i m_clickPointOffset{0, 0};

};

#endif