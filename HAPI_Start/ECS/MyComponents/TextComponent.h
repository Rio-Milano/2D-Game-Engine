#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include"../Base/BaseComponent.h"
#include<string>
#include<memory>
#include<HAPI_lib.h>
using namespace HAPISPACE;
#include"../DataStructures/Vector.h"

struct TextComponent final : public BaseComponent
{
	void InitializeTextComponent(const HAPI_TColour& fillColour, const std::string& text, const int& textSize = 12, const unsigned int& styleFlags = HAPI_TextStyle::eRegular)
	{
		m_fillColour = fillColour;
		m_text = text;
		m_textSize = textSize;
		m_styleFlags = styleFlags;
	};


	HAPI_TColour m_fillColour;
	std::string m_text;
	int m_textSize;
	unsigned int m_styleFlags;
	uint32_t clipToEntityEventRectID{0};
	Vector2f m_offsetFromTransform{0.0f, 0.0f};

};

#endif