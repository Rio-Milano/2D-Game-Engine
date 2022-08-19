#ifndef LISTBOXWIDGET_H
#define LISTBOXWIDGET_H

#include"../UI Core/Widget.h"

#include"../DataStructures/Vector.h"

#include<list>
#include<string>
#include<HAPI_lib.h>
using namespace HAPISPACE;

class ListBoxWidget final : public Widget
{
	friend class ListBoxWidgetSlideBarScript;
public:

	ListBoxWidget() = default;
	~ListBoxWidget() = default;

	void CreateWidget(const std::string& widgetName, RenderManager& renderManager, UI_Manager& uiManager, ECS_Engine& ecs) override;

	void HideWidget(ECS_Engine& ecs, RenderManager& renderManager);
	void ShowWidget(ECS_Engine& ecs, RenderManager& renderManager);

	void TranslateWidget(const Vector2f& translateTo, ECS_Engine& ecsEngineRef)const;
	void ScrollWidgets(ECS_Engine& ecs, const float& moveWidgetsByY);

	uint32_t UniformWidgetConfig(const std::string& widgetName, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef);

	uint32_t AddHeadding(const std::string& headdingNameArgRef, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef);
	uint32_t AddSubHeadding(const std::string& subHeaddingNameArgRef, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef);
	uint32_t AddTextElement(const std::string& textWidgetArgRef, RenderManager& renderManagerRef, UI_Manager& uiManager, ECS_Engine& ecsEngineRef);


	//horizontal offset calculations
	void SetHeaddingOffset(const std::string& headdingOffsetArgRef) { this->m_headdingOffset = headdingOffsetArgRef; };
	void SetSubHeaddingOffset(const std::string& subHeaddingOffsetArgRef) { this->m_subHeaddingOffset = subHeaddingOffsetArgRef; };
	void SetTextElementOffset(const std::string& textElementOffsetArgRef) { this->m_textElementOffset = textElementOffsetArgRef; };


private:
	std::list<uint32_t> m_listOfWidgets{};

	HAPI_TColour
		m_headdingColor{ HAPI_TColour::WHITE },
		m_subHeaddingColor{ HAPI_TColour::BLUE },
		m_textElementColor{ HAPI_TColour::HORRID_PINK };

	int
		m_headdingTextSize{ 20 },
		m_subHeaddingTextSize{ 23 },
		m_textElementTextSize{ 17 },

		m_headdingTextStyle{ HAPI_TextStyle::eBold },
		m_subHeaddingTextStyle{ HAPI_TextStyle::eItalic },
		m_textElementTextStyle{ HAPI_TextStyle::eRegular };

	std::string
		m_headdingOffset{""},
		m_subHeaddingOffset{""},
		m_textElementOffset{""};

	const int m_defaultTextElementHeight{50};
};

#endif