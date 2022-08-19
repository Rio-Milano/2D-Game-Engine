#include "Renderer.h"
#include"../Macros/Colors.h"

#include"../Core/Visualisation.h"
#include"../Utils/Utils.h"

#include<assert.h>

int Renderer::m_ScreenWidth{};
int Renderer::m_ScreenHeight{};

int Renderer::m_CentreXPos{};
int Renderer::m_CentreYPos{};

size_t Renderer::m_TotalBytesOnScreen{};

IntRect Renderer::m_ScreenRect{};

float Renderer::m_eyeDistance{};

const int Renderer::InitHAPIWindow(Vector2i& windowSize, std::string& windowName, const int& windowInitFlags)
{
	//check for init fail
	if (!HAPI.Initialise(windowSize.x, windowSize.y, windowName, windowInitFlags))
		return false;

	m_screenPtr = HAPI.GetScreenPointer();
	HAPI.SetShowFPS(1, 890, 0, RED);
	UpdateWindowAttribs(static_cast<const Vector2i&>(windowSize));
	//HAPI.LimitFrameRate(10);
	return true;

}

void Renderer::UpdateWindowAttribs(const Vector2i& screenSize)
{
	m_ScreenWidth = screenSize.x;
	m_ScreenHeight = screenSize.y;

	m_CentreXPos = m_ScreenWidth / 2;
	m_CentreYPos = m_ScreenHeight / 2;

	m_TotalBytesOnScreen =  static_cast<size_t>(static_cast<long long int>(m_ScreenWidth) * static_cast<long long int>(m_ScreenHeight) * static_cast<long long int>(m_kBytesPerPixel));


	m_ScreenRect.x1 = 0;
	m_ScreenRect.y1 = 0;

	m_ScreenRect.x2 = m_ScreenWidth-1;
	m_ScreenRect.y2 = m_ScreenHeight - 1;
}

void Renderer::ClearScreenColor(const HAPI_TColour& color, const bool& useMonochrome)
{
	if (useMonochrome)
		ClearScreenColorMonochrome(color.red);
	else
		ClearScreenColorRGBA(color);
}

#pragma region ClearScreen
//loop through every 4 bytes of the screen buffer and copy 4 bytes from the passed color into the memory buffer + offset 
void Renderer::ClearScreenColorRGBA(const HAPI_TColour& color) const
{
	size_t i{0};
	for (; i < m_TotalBytesOnScreen; i += m_kBytesPerPixel)
		memcpy(m_screenPtr + i, &color, m_kBytesPerPixel);
}
//able to set color in range of black to white. Small color val = blacker  Bigger color val = whiter.. Note alpfa value will move with value
void Renderer::ClearScreenColorMonochrome(const BYTE& color) const
{
	assert(m_screenPtr != nullptr);

	memset(m_screenPtr, color, static_cast<long long int>(m_ScreenHeight) * static_cast<long long int>(m_ScreenWidth) * static_cast<long long int>(4));
}
#pragma endregion

#pragma region PixelSetters
void Renderer::SetPixel(const Vector2i& position, const HAPI_TColour& color, const bool& useAlphaChannel, const bool& useMonochrome) const
{
	assert(m_screenPtr != nullptr);

	if (ValidatePixelPosition(position))
	{
		if (useMonochrome)
			SetPixelMonochrome(position, color.red);
		else if (useAlphaChannel)
			SetPixelRGBA(position, color);
		else
			SetPixelRGB(position, color);
	}
	
}


//sets a single pixel to a color
void Renderer::SetPixelRGB(const Vector2i& position, const HAPI_TColour& color) const
{
	assert(m_screenPtr != nullptr);

	int pixelOffset{ this->GetPixelOffset(position) };//get offset from screen pointer
	BYTE* pixelPos{ m_screenPtr + pixelOffset };//add offset to screen ptr

	memcpy(pixelPos, &color, m_kBytesPerPixel);//coppy the color into the pixel
}

void Renderer::SetPixelRGBA(const Vector2i& position, const HAPI_TColour& color) const
{
	assert(m_screenPtr != nullptr);

	int pixelOffset{ this->GetPixelOffset(position) };
	BYTE* pixelPos{ m_screenPtr + pixelOffset };

	BYTE alpha = color.alpha;
	
	if (alpha == 255)
		SetPixelRGB(position, color);
	else if(alpha > 0)
	{
		pixelPos[0] = pixelPos[0] + ((alpha * (color.red - pixelPos[0])) >> 8);
		pixelPos[1] = pixelPos[1] + ((alpha * (color.green - pixelPos[1])) >> 8);
		pixelPos[2] = pixelPos[2] + ((alpha * (color.blue   - pixelPos[2])) >> 8);
	}
}

//same as SetPixelRGBA() but uses 1 value for color
void Renderer::SetPixelMonochrome(const Vector2i& position, const BYTE& color) const
{
	assert(m_screenPtr != nullptr);

	int pixelOffset{ this->GetPixelOffset(position) };
	BYTE* pixelPos{ m_screenPtr + pixelOffset };
	
	memset(pixelPos, color, m_kBytesPerPixel);
}

//sets a range of pixels in a square or rectangle from the pixel position
void Renderer::SetBlockOfPixles(const Vector2i& position, const Vector2i& size, const HAPI_TColour& color, const bool& useAlphaChannel, const bool& useMonochrome) const
{
	assert(m_screenPtr != nullptr);


	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			if (ValidatePixelPosition(Vector2i(x, y)))
				SetPixel(Vector2i(position.x + x, position.y + y), color, useAlphaChannel, useMonochrome);
		}
	}
}
#pragma endregion





void Renderer::DrawLine(const Vector2i& startPosition, const int& length, const bool& vertical, const HAPI_TColour& color, const bool& useRGBA, const bool& useMonochrome) const
{
	assert(m_screenPtr != nullptr);

	for (int i{ 0 }; i < length; i++)
	{
		Vector2i currentPosition = startPosition;

		if (vertical)
			currentPosition.y += i;
		else
			currentPosition.x += i;

		if(ValidatePixelPosition(currentPosition))
			SetPixel(currentPosition, color, useRGBA, useMonochrome);
	}
}


void Renderer::DrawEditorGrid(const Vector2i& cameraPosition, const Vector2i& lineEveryXYUnits, const HAPI_TColour& color, const bool& useRGBA, const bool& useMonochrome) const
{
	int startX{lineEveryXYUnits.x - (cameraPosition.x % lineEveryXYUnits.x)};
	int startY{lineEveryXYUnits.y -  (cameraPosition.y % lineEveryXYUnits.y) };

	startX -= (startX % lineEveryXYUnits.x) * lineEveryXYUnits.x;
	startY -= (startY % lineEveryXYUnits.y) * lineEveryXYUnits.y;

	for (int xAxisLine{ startX }; xAxisLine < m_ScreenWidth; xAxisLine += lineEveryXYUnits.x)
		DrawLine(Vector2i(xAxisLine, 0), m_ScreenHeight, true, color, useRGBA, useMonochrome);
	
	for (int YAxisLine{ startY }; YAxisLine < m_ScreenHeight; YAxisLine += lineEveryXYUnits.y)
		DrawLine(Vector2i(0, YAxisLine), m_ScreenWidth, false, color, useRGBA, useMonochrome);
}

void Renderer::DrawRectangle(const IntRect& rect, const HAPI_TColour& color, const bool& useAlphaChannel, const bool& useMonochrome) const
{
	const int
		rectangleWidth{ rect.x2 - rect.x1 },
		rectangleHeight{rect.y2 - rect.y1};

	DrawLine(Vector2i(rect.x1, rect.y1), rectangleWidth, false, color, useAlphaChannel, useMonochrome);
	DrawLine(Vector2i(rect.x1, rect.y2), rectangleWidth, false, color, useAlphaChannel, useMonochrome);


	DrawLine(Vector2i(rect.x1, rect.y1), rectangleHeight, true, color, useAlphaChannel, useMonochrome);
	DrawLine(Vector2i(rect.x2, rect.y1), rectangleHeight, true, color, useAlphaChannel, useMonochrome);

}

void Renderer::DrawCircle( Vector2i position, const int& radius, const HAPI_TColour& color)const
{
	const double
		PI { 3.1415926535};

	double 
		pixel{0},
		angle{},
		x_{},
		y_{};

	if (radius == 0)
		return;

	for (; pixel < 360; pixel += 0.1)
	{
		angle = pixel;
		x_ = radius * cos(angle * PI / 180);
		y_ = radius * sin(angle * PI / 180);
		SetPixel(Vector2i(static_cast<int>(static_cast<double>(position.x) + x_), static_cast<int>(static_cast<double>(position.y) + y_)), color, false, false);
	}
	//recursively draw a smaller circle until the radius is 0
	return DrawCircle(Vector2i(position.x, position.y), radius-1, color);
}

void Renderer::DrawText(const Vector2i& position, const HAPI_TColour& fillColour, const std::string& text, const int& textSize, const unsigned int& styleFlags)const
{
	HAPI.RenderText(position.x, position.y, fillColour, text, textSize, styleFlags);
}

void Renderer::DrawTextClip(
	const Vector2i& position, const HAPI_TColour& fillcolor, const HAPI_TColour& outlineColor,
	const float& outlineThickness, const std::string& text, const int& textSize, const unsigned int& styleFlags,
	const int& clipLeft, const int& clipRight, const int& clipTop, const int& clipBottom)
{

	HAPI.RenderText(position.x, position.y, fillcolor, outlineColor, outlineThickness, text, textSize, styleFlags,
		Utils::ClampValue<int>(clipLeft, 0, m_ScreenWidth-1), 
		Utils::ClampValue<int>(clipRight, 0, m_ScreenWidth - 1),
		Utils::ClampValue<int>(clipTop, 0, m_ScreenHeight - 1),
		Utils::ClampValue<int>(clipBottom, 0, m_ScreenHeight - 1));
}

void Renderer::DrawTexture(Vector2i position, Sprite& sprite)
{
	/*
	- remember, the rect is the texture rect and is local to the texture not the world
	*/


	//make a new rect to manipulate
	IntRect worldRect = sprite.m_textureRef->m_textureRect;

	//transform the world rect to the draw call position
	/*
	- remember, the position is not mapped to 0->screenAxisMax so if clipping needed we must map this value to line up with clipped rect
	*/
	worldRect.Translate(position);

	//if the screen rect fully contains the world rect
	if (m_ScreenRect.IsRectInside(worldRect))
	{
		//if the texture failed to load then call fast blit 
		/*
		- remember, if textures fail to load they are filled with a buffer of pink RGBA values of equal size to the authentic counterpart
		*/
		if (sprite.m_textureRef->m_textureFailedToLoad)
			BlitTextureFast(position, sprite.m_textureRef->m_textureBuffer, sprite.m_textureRef->m_textureSize.x, sprite.m_textureRef->m_textureSize.y, sprite.m_effects, 0);

		//if using alpha effect blit with alpha
		else if (sprite.m_useAlphaChannels)
			BlitTextureAlpha(position, sprite.m_textureRef->m_textureBuffer, sprite.m_textureRef->m_textureSize.x, sprite.m_textureRef->m_textureSize.y, sprite.m_effects, 0);
		//if not using alpha effcts then fast blit
		else
			BlitTextureFast(position, sprite.m_textureRef->m_textureBuffer, sprite.m_textureRef->m_textureSize.x, sprite.m_textureRef->m_textureSize.y, sprite.m_effects, 0);
	}
	//if the world rect is intersecting with the screen then the world rect needs clipping
	else if (m_ScreenRect.IsRectIntersecting(worldRect))
	{
		//clip the world rect
		worldRect.ClampRect(m_ScreenRect);

		//calculate the offset made by the texture rect and the world rect
		int
		widthOfWorldRect{ worldRect.x2 - worldRect.x1 },
		heightOfWorldRect{ worldRect.y2 - worldRect.y1 },

		heightOfTextureRect{ sprite.m_textureRef->m_textureRect.y2 - sprite.m_textureRef->m_textureRect.y1 },
		widthtOfTextureRect{ sprite.m_textureRef->m_textureRect.x2 - sprite.m_textureRef->m_textureRect.x1 };


		unsigned int 
		xOffsetWorldRectTextureRect{ static_cast<unsigned int>(worldRect.x1 - position.x) },
		yOffsetWorldRectTextureRect{ static_cast<unsigned int>(worldRect.y1 - position.y) },

		offsetFromTextureRectToWorldRectInBytes{ (xOffsetWorldRectTextureRect + yOffsetWorldRectTextureRect * widthtOfTextureRect) * m_kBytesPerPixel },
		endOfTextureLineOffset = widthtOfTextureRect - widthOfWorldRect;

		//update the rendering position
		position.x = worldRect.x1;
		position.y = worldRect.y1;

		if (sprite.m_textureRef->m_textureFailedToLoad)
			BlitTextureFast(position, sprite.m_textureRef->m_textureBuffer + offsetFromTextureRectToWorldRectInBytes, widthOfWorldRect, heightOfWorldRect, sprite.m_effects, endOfTextureLineOffset);

		//if using alpha effect blit with alpha
		else if (sprite.m_useAlphaChannels)
			BlitTextureAlpha(position, sprite.m_textureRef->m_textureBuffer + offsetFromTextureRectToWorldRectInBytes, widthOfWorldRect, heightOfWorldRect, sprite.m_effects, endOfTextureLineOffset);
		//if not using alpha effcts then fast blit
		else
			BlitTextureFast(position, sprite.m_textureRef->m_textureBuffer + offsetFromTextureRectToWorldRectInBytes, widthOfWorldRect, heightOfWorldRect, sprite.m_effects, endOfTextureLineOffset);


	}
	else
	{
		/*
		The rect is fully outside the world
		*/
		return;
	}
}



void Renderer::BlitTextureFast(const Vector2i& position, BYTE* textureBuffer, const int& textureWidth, const  int& textureHeight, const unsigned int& effects, const unsigned int& endOfLineOffset)const
{
	assert(m_screenPtr != nullptr);


	//might add a clipped variant of this function instead of a generic one so less calculations are done

	int pixelOffset{ GetPixelOffset(position) };
	BYTE* tempScreen{ this->m_screenPtr + pixelOffset };

	for (int textureRow{ 0 }; textureRow < textureHeight; textureRow++)
	{
		memcpy(tempScreen, textureBuffer, textureWidth * m_kBytesPerPixel);
		
		tempScreen += this->m_ScreenWidth * static_cast<int>(this->m_kBytesPerPixel);
		textureBuffer += (textureWidth + static_cast<int>(endOfLineOffset)) * static_cast<int>(this->m_kBytesPerPixel);
	}
}

void Renderer::BlitTextureAlpha(const Vector2i& position, BYTE* textureBuffer, const int& textureWidth, const  int& textureHeight, const unsigned int& effects, const unsigned int& endOfLineOffset)
{
	assert(m_screenPtr != nullptr);

	int pixelOffset{ GetPixelOffset(position) };
	BYTE* tempScreen{this->m_screenPtr + pixelOffset };

	for (int yPixelPos = 0; yPixelPos < textureHeight; yPixelPos++)
	{
		for (int xPixelPos = 0; xPixelPos < textureWidth; xPixelPos++)
		{
			if (textureBuffer[3] == 255)
				memcpy(tempScreen,  textureBuffer, m_kBytesPerPixel);
			else if(textureBuffer[3] > 0)
			{

				//Slow version as floating point calculations take longer to do than bit shifting calculations 
				// 
				//alphaModulator = static_cast<float>(currentTexelPixel[3]) / 255;
				/*currentScreenPixel[0] = alphaModulator * currentTexelPixel[0] + (1.0f - alphaModulator) * currentScreenPixel[0];
				currentScreenPixel[1] = alphaModulator * currentTexelPixel[1] + (1.0f - alphaModulator) * currentScreenPixel[1];
				currentScreenPixel[2] = alphaModulator * currentTexelPixel[2] + (1.0f - alphaModulator) * currentScreenPixel[2];*/

				//Fast version as more calculations needed per pixel
				// 
				BYTE alpha{0};

				if (effects & Effects::UseGlobalAlpha)
					BYTE alpha = static_cast<BYTE>(textureBuffer[3] * m_variedAlphaChange / 255.0f);
				else
					alpha = textureBuffer[3];

				tempScreen[0] = tempScreen[0] + ((alpha * (textureBuffer[0] - tempScreen[0])) >> 8);
				tempScreen[1] = tempScreen[1] + ((alpha * (textureBuffer[1] - tempScreen[1])) >> 8);
				tempScreen[2] = tempScreen[2] + ((alpha * (textureBuffer[2] - tempScreen[2])) >> 8);
			}
			textureBuffer += this->m_kBytesPerPixel;
			tempScreen += this->m_kBytesPerPixel;
		}
		textureBuffer += endOfLineOffset * this->m_kBytesPerPixel;
		tempScreen += (this->m_ScreenWidth - textureWidth) * this->m_kBytesPerPixel;
	}
	if (effects & Effects::UseGlobalAlpha)
	{
		if ((m_variedAlphaChangeVelocity > 0.0f && m_variedAlphaChange >= 255.0f) || (m_variedAlphaChangeVelocity < 0.0f && m_variedAlphaChange <= 0.0f))
			m_variedAlphaChangeVelocity = -m_variedAlphaChangeVelocity;
	

		m_variedAlphaChange += m_variedAlphaChangeVelocity*Visualisation::deltaTime;

	}
}
//calculate the prespective projection of the star position
Vector2i Renderer::Point3DPrespectiveProjection(const Vector3f& position) const
{
	if (position.z == 0.0f)
		return Vector2i(-1, -1);

	int screenX = static_cast<int>(((m_eyeDistance * (position.x - static_cast<float>(m_CentreXPos))) / (m_eyeDistance + position.z)) + static_cast<float>(m_CentreXPos));
	int screenY = static_cast<int>(((m_eyeDistance * (position.y - static_cast<float>(m_CentreYPos))) / (m_eyeDistance + position.z)) + static_cast<float>(m_CentreYPos));

	return Vector2i(screenX, screenY);
}



//find the offset of a screen position
const int Renderer::GetPixelOffset(const Vector2i& position) const
{
	return (position.x + position.y * m_ScreenWidth) * static_cast<int>(m_kBytesPerPixel);
}

//if pixel pos is greater than or equal to the max amount of bytes in the screen buffer then the pixel pos is out of bounds of the screen
const bool Renderer::ValidatePixelPosition(const Vector2i& position) const
{
	if (position.x < 0 || position.x >= m_ScreenWidth || position.y < 0 || position.y >= m_ScreenHeight)
		return false;
	else
		return true;
}

