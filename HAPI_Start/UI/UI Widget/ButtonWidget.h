#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include<HAPI_lib.h>
using namespace HAPISPACE;

#include"../UI Core/Widget.h"
#include"../DataStructures/Vector.h"

class ButtonWidget final: public Widget
{
public:
	/*
	Add button text and text color and text name
	*/
	ButtonWidget() : Widget() {}
	~ButtonWidget() = default;

	/*
	As there is pre-requisite data required to create the button such as button size, We cant first add the button to the UI manager
	as this will call the create widget function. So we need to have a setup function that will setup some pre-requisite data first
	*/
	void CreateWidget(const std::string& widgetName, RenderManager& renderManager, UI_Manager& uiManager, ECS_Engine& ecs) override;
	 
	

	void ResetWidget(ECS_Engine& ecs) override;


	void InitializeButton(
		ECS_Engine& ecs,
		const Vector2f& position, 
		const Vector2i& buttonSize, 
		const HAPI_TColour& buttonFillColor,
		const HAPI_TColour& buttonTextColour,
		const std::string& buttonText,
		const Vector2f textOffset,
		const unsigned int textSize = 12
	);

	bool OneStepClick(ECS_Engine& ecs);

};

#endif
