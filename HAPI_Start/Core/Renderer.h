#ifndef RENDERER_H
#define RENDERER_H

#include<HAPI_lib.h>
using namespace HAPISPACE;

#include"../DataStructures/Vector.h"
#include"Effects.h"
#include"../GeneralObjects/Sprite.h"
#include"../GeneralObjects/Rect.h"
#include"../ECS/Base/ECS_Engine.h"


class Renderer final
{
public:

	Renderer() = default;

	~Renderer() = default;

	#pragma region HAPI_Window_And_Init_Functions
	const int InitHAPIWindow(Vector2i& windowSize, std::string& windowName, const int& windowInitFlags);
	void UpdateWindowAttribs(const Vector2i& screenSize);
	#pragma endregion

	//No alpha channel support
	#pragma region ClearScreen
		void ClearScreenColor(const HAPI_TColour& color = HAPI_TColour(0, 0, 0, 0), const bool& useMonochrome = true);

	#pragma endregion

	//Alpha channel not supported when using monochrome option. When using monochrome option the red channel is only used
	#pragma region PixelSetters
		//Decision function to set pixel using passed pixel setter preferences
		void SetPixel(const Vector2i& position, const HAPI_TColour& color, const bool& useAlphaChannel, const bool& useMonochrome)const;

		//Draws a collection of pixels extending by a given size. 
		void SetBlockOfPixles(const Vector2i& position, const Vector2i& size, const HAPI_TColour& color, const bool& useAlphaChannel, const bool& useMonochrome)const;
	#pragma endregion

	#pragma region GeometricShapes 
		//draw a line horizontally or vertically || using false for useRGB will use monochrome setter with red component of color parameter
		void DrawLine(const Vector2i& startPosition, const  int& length, const bool& vertical, const HAPI_TColour& color, const bool& useAlphaChannel, const bool& useMonochrome)const;
		void DrawEditorGrid(const Vector2i& cameraPosition, const Vector2i& lineEveryXYUnits, const HAPI_TColour& color, const bool& useAlphaChannel, const bool& useMonochrome)const;


		void DrawRectangle(const IntRect& rect, const HAPI_TColour& color, const bool& useRGBA, const bool& useMonochrome)const;


		void DrawCircle( Vector2i position, const int& radius, const HAPI_TColour& color)const;
	#pragma endregion

	#pragma region ComplexGraphics
		void DrawText(const Vector2i& position, const HAPI_TColour& fillColour, const std::string& text, const int& textSize = 12, const unsigned int& styleFlags = HAPI_TextStyle::eRegular)const;

		void DrawTextClip(const Vector2i& position, const HAPI_TColour& fillcolor, const HAPI_TColour& outlineColor, 
			const float& outlineThickness, const std::string& text, const int& textSize = 12, const unsigned int& styleFlags = HAPI_TextStyle::eRegular,
			const int& clipLeft = 0, const int& clipRight = 0, const int& clipTop = 0, const int& clipBottom =  0);

		void DrawTexture(Vector2i position, Sprite& sprite);
	#pragma endregion

	#pragma region HelperFunctions
		//pixel projection
		Vector2i Point3DPrespectiveProjection(const Vector3f& position)const;
	#pragma endregion



private:
	#pragma region ClearScreenImplementation
		/*
		Clears the screen by overwriting RGBA channels with provided color
		*/
		void ClearScreenColorRGBA(const HAPI_TColour& color) const;
		/*
		Clears the screen by overwriting all channels with a single value
		*/
		void ClearScreenColorMonochrome(const BYTE& color) const;
	#pragma endregion

	#pragma region PixelSetterImplementation
		void SetPixelRGB(const Vector2i& position, const HAPI_TColour& color) const;
		void SetPixelRGBA(const Vector2i& position, const HAPI_TColour& color) const;
		void SetPixelMonochrome(const Vector2i& position, const BYTE& color) const;
	#pragma endregion

	#pragma region InnerHelperFunctions
		//calculate where a pixel is in the screen buffer
		const int GetPixelOffset(const Vector2i& position) const;
		//check of a pixel is pointing outside the screen buffer
		const bool ValidatePixelPosition(const Vector2i& position) const;
	#pragma endregion

	#pragma region ComplexGraphicsImplementation
	//blit texture draw
	void BlitTextureFast(const Vector2i& position, BYTE* textureBuffer, const int& textureWidth, const  int& textureHeight, const unsigned int& effects, const unsigned int& endOfLineOffset = 0)const;

	void BlitTextureAlpha(const Vector2i& position, BYTE* textureBuffer, const int& textureWidth, const  int& textureHeight, const unsigned int& effects, const unsigned int& endOfLineOffset = 0);
#pragma endregion


public:
	#pragma region ScreenData
	static int
		m_ScreenWidth,
		m_ScreenHeight,

		m_CentreXPos,
		m_CentreYPos;

	static size_t
		m_TotalBytesOnScreen;
	
	static IntRect 
		m_ScreenRect;
#pragma endregion

	static float
		m_eyeDistance;

	#pragma region Constants
	static constexpr size_t
		m_kBytesPerPixel{ 4 };

	static constexpr float
		m_maxEyeDistance{ 4000.0f };
#pragma endregion

private:
	BYTE* m_screenPtr{nullptr};

	float m_variedAlphaChange{ 0.0f };
	float m_variedAlphaChangeVelocity{ 0.5f };
};

#endif