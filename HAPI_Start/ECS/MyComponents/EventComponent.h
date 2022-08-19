#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H

#include"../ECS/Base/BaseComponent.h"

#include"../GeneralObjects/Rect.h"

struct EventComponent final : public BaseComponent
{
	void InitializeEventComponent(const IntRect& eventRectArg)
	{
		m_eventRect = eventRectArg;
	}
	 
	bool
		m_isMouseOver{ false },
		m_isMouseLeftClick{ false },
		m_tagComponent{ false },
		m_mouseLeftClickHolding{ false };
	int32_t parentID{-2};

	IntRect m_eventRect;
};

#endif