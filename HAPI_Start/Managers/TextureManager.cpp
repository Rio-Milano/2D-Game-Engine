#include "TextureManager.h"

#include<HAPI_lib.h>
#include"../DataStructures/Vector.h"
#include"../GeneralObjects/Texture.h"


TextureManager::TextureManager()
{
}


std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& textureName)
{
	//if the texture is NOT in the map
	if (m_textureNameToTextureBufferHM.find(textureName) == m_textureNameToTextureBufferHM.end())
	{
		std::cerr << std::string("Texture :" + textureName + " was not found\n");
		return nullptr;
	}
	return m_textureNameToTextureBufferHM[textureName];
}

bool TextureManager::LoadTextureAndStore(const std::string& fileDir, const std::string& textureName, const size_t& bytesPerPixel)
{
	std::shared_ptr<Texture> newTexture{LoadTexture(fileDir, bytesPerPixel)};
	m_textureNameToTextureBufferHM[textureName] = newTexture;

	return newTexture->m_textureFailedToLoad;
}

std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& fileDir, const size_t& bytesPerPixel)
{
	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
	int textureWidth{}, textureHeight{};
	if (!HAPI.LoadTexture(fileDir, &newTexture->m_textureBuffer, textureWidth, textureHeight))
	{
		newTexture->m_textureFailedToLoad = true;
		FillNullTextureBuffer(&newTexture->m_textureBuffer, Vector2i(textureWidth, textureHeight), bytesPerPixel);
		m_runTimeErrors.append("Texture at location :" + fileDir + " failed to load!\n");
		std::cerr << "Failed to load texture at location :" << fileDir << "\n";
	}

	newTexture->m_textureSize.x = textureWidth;
	newTexture->m_textureSize.y = textureHeight;

	newTexture->m_textureRect.x1 = 0;
	newTexture->m_textureRect.y1 = 0;
	newTexture->m_textureRect.x2 = textureWidth;
	newTexture->m_textureRect.y2 = textureHeight;


	return newTexture;
}

void TextureManager::UnloadTextureFromMemory(const std::string& textureName)
{
	if (m_textureNameToTextureBufferHM.find(textureName) == m_textureNameToTextureBufferHM.end())
	{
		std::cerr << std::string("Can not unload texture with the name :" + textureName + " because texture was not found!\n");
		return;
	}
	m_textureNameToTextureBufferHM.erase(textureName);
}

void TextureManager::LoadTextureFromTextureAndStore(const std::string& baseTextureName, const IntRect& subTextureBounds, const std::string& derivedTextureName, const size_t& bytesPerPixel)
{
	//if base texture is not found then report error
	if (m_textureNameToTextureBufferHM.find(baseTextureName) == m_textureNameToTextureBufferHM.end())
	{
		std::cerr << std::string("There is no texture with the name :" + baseTextureName + "\n");
		return;
	}

	//if there is already a texture with the derived texture name then report error
	if (m_textureNameToTextureBufferHM.find(derivedTextureName) != m_textureNameToTextureBufferHM.end())
	{
		std::cerr << std::string("There is already a texture with the name :" + derivedTextureName + "\n");
		return;
	}

	//create an editable IntRect from subTextureRect
	IntRect subTextureBoundsMutable{subTextureBounds};
	
	if (!m_textureNameToTextureBufferHM[baseTextureName]->m_textureRect.IsRectInside(subTextureBoundsMutable))
	{
		std::cerr << std::string("Derived Texture can not be created as rect is outside of the texture bounds\n");
		return;
	}

	//define some derived attribs for readability
	int
		rectHeight{ subTextureBoundsMutable.y2 - subTextureBoundsMutable.y1 },
		rectWidth{ subTextureBoundsMutable.x2 - subTextureBoundsMutable.x1 };

	//create a new texture buffer and fill it with enough bytes to hold the derived texture
	BYTE* newTextureBuffer{new BYTE[(rectHeight * rectWidth) * bytesPerPixel]};

	//get const ref pointer to base texture data
	BYTE*
		baseTexturePtr{m_textureNameToTextureBufferHM[baseTextureName]->m_textureBuffer};

	//define the offset in bytes to the derived texture
	int 
		offsetToDerivedTexture{((subTextureBoundsMutable.x1 + subTextureBoundsMutable.y1 * m_textureNameToTextureBufferHM[baseTextureName]->m_textureSize.x)* static_cast<int>(bytesPerPixel))};

	//loop through each row of the derived texture
	for (int height{ 0 }; height < rectHeight; height++)
	{ 
		memcpy(
			//from the new texture add on widths coppied so far in bytes
			newTextureBuffer + ((height * rectWidth)*bytesPerPixel),
			//from the base texture add on the offset then widths coppied from in bytes
			baseTexturePtr + offsetToDerivedTexture + (height * m_textureNameToTextureBufferHM[baseTextureName]->m_textureSize.x)*bytesPerPixel,
			//the amount coppied is the width relative to bytes per pixel
			rectWidth * bytesPerPixel
		);
	}

	//translate the rect into local space
	subTextureBoundsMutable.Translate(Vector2i(0, 0));

	//make a new rect object
	std::shared_ptr<Texture> newTexture{std::make_shared<Texture>()};
	//assign the new texure buffer to the texture
	newTexture->m_textureBuffer = newTextureBuffer;
	//assign the texture rect to the texture(now in local space)
	newTexture->m_textureRect = subTextureBoundsMutable;
	//assign the texture size to the textre
	newTexture->m_textureSize = Vector2i(rectWidth, rectHeight);

	//add the texure to local space
	m_textureNameToTextureBufferHM[derivedTextureName] = newTexture;




}

const std::string& TextureManager::GetRunTimeErrors() const
{
	return m_runTimeErrors;
}

void TextureManager::FillNullTextureBuffer(BYTE** textureBuffer, const Vector2i& textureSize, const size_t& bytesPerPixel) const
{
	int NumOfBytes = static_cast<int>((textureSize.x * textureSize.y) * bytesPerPixel);

	*textureBuffer = new BYTE[NumOfBytes];

	for (int i = 0; i < textureSize.x * textureSize.y; i++)
		memcpy(*(textureBuffer)+i * bytesPerPixel, &HAPI_TColour::HORRID_PINK, bytesPerPixel);
}

void TextureManager::FillBufferWithColor(BYTE* buffer, const Vector2i& textureSize, const HAPI_TColour& textureColour)
{
	BYTE* tempBuffer{buffer};
	for (int yPos{ 0 }; yPos < textureSize.y; yPos++)
	{
		for (int xPos{ 0 }; xPos < textureSize.x; xPos++)
		{
			memcpy(tempBuffer, &textureColour, 4);
			tempBuffer += 4;
		}
	}

}

std::shared_ptr<Texture> TextureManager::MakeTexture(BYTE* texturePtr, const Vector2i& textureSize) const
{
	std::shared_ptr<Texture> newTexture{std::make_shared<Texture>()};
	newTexture->m_textureBuffer = texturePtr;
	//origin will be init to 0
	newTexture->m_textureRect.x2 = textureSize.x;
	newTexture->m_textureRect.y2 = textureSize.y;
	newTexture->m_textureSize = textureSize;

	return newTexture;
}
