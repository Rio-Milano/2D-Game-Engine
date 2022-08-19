#ifndef Texture_Manager_H
#define TextureManager_H

#include <unordered_map>
#include<string>


#include"../DataStructures/Vector.h"
#include"../GeneralObjects/Texture.h"

/*
this class embraces the use of the flyweight pattern by managing textures needed in one place
*/

class TextureManager final /*: public Singleton<TextureManager>*/
{
public:

	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;			
	};

		 
	//get a texture by name
	std::shared_ptr<Texture> GetTexture(const std::string& textureName);

	//load a texture into memory
	bool LoadTextureAndStore(const std::string& fileDir, const std::string& textureName, const size_t& bytesPerPixel);
	std::shared_ptr<Texture> LoadTexture(const std::string& fileDir, const size_t& bytesPerPixel);

	void UnloadTextureFromMemory(const std::string& textureName);

	void LoadTextureFromTextureAndStore(const std::string& baseTextureName, const IntRect& subTextureBounds, const std::string& derivedTextureName, const size_t& bytesPerPixel);


	const std::string& GetRunTimeErrors() const;

	
	void FillNullTextureBuffer(BYTE** textureBuffer, const Vector2i& textureSize, const size_t& bytesPerPixel) const;
	void FillBufferWithColor(BYTE* buffer, const Vector2i& textureSize, const HAPI_TColour& textureColour);
	std::shared_ptr<Texture> MakeTexture(BYTE* texturePtr, const Vector2i& textureSize)const;

private:
	TextureManager();
	~TextureManager() = default;


	//hash map to map names of texures to textureBuffer
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureNameToTextureBufferHM;

	std::string m_runTimeErrors{};

};

#endif

