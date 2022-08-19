#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include"../UI/UI Core/Widget.h"

class TextWidget final: public Widget
{
public:
	TextWidget() = default;
	~TextWidget() = default;

	void CreateWidget(const std::string& widgetName, RenderManager& renderManager, UI_Manager& uiManager, ECS_Engine& ecs)override;

};

#endif