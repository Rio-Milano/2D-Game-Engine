#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include"../ECS/Base/BaseComponent.h"


struct RenderComponent final : public BaseComponent
{
	enum RenderConfigs : unsigned int
	{
		NoConfigs = 0,
		DrawBoundingBox = 1 << 0,
		SquareShape = 1 << 1,
		Sprite = 1 << 2,
		Text = 1 << 3,
		Line = 1 << 4,//not yet suported
		DrawBoundingBoxCollisionComponent = 1 << 5,
		DrawBoundBoxEventComponent = 1 << 6,
		ClipTextToEventRect = 1 << 7
	};

	enum RenderLayer : unsigned int
	{
		Default =		0,
		Background =	1,
		Tile =			2,
		WorldObject_1 =	3,
		WorldObject_2 = 4,
		WorldObject_3 = 5,
		UI =	6,
		DoNotUse_TotalRenderLayers = 7
	};


	void InitializeRenderComponent(
		const unsigned int& configs,
		const unsigned int& renderLayer,
		const bool& isActive,
		const bool& worldRender
	)
	{
		m_renderConfigs = configs;
		m_renderLayer = renderLayer;
		m_isActive = isActive;
		m_WorldRender = worldRender;
	}

	unsigned int 
		m_renderConfigs,
		m_renderLayer;

	bool 
		m_isActive{true},
		m_WorldRender{true},
		m_onlyRenderWhenInEditorMode{ false },
		m_onlyRenderWhenInWorldMode{false};

	//The placement in the render layer list
	int32_t m_renderLayerPrecedence{-1};
};

#endif