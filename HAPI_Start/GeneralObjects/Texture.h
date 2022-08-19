#ifndef TEXTURE_H
#define TEXTURE_H

#include<HAPI_Types.h>
#include"../DataStructures/Vector.h"
#include"Rect.h"

using BYTE = unsigned char;
struct Texture final
{

	BYTE* m_textureBuffer{nullptr};//buffer
	Vector2i m_textureSize{};//[widthm, height]
	IntRect m_textureRect;//[0,0], [width, height]

	bool m_textureFailedToLoad{false};

	~Texture()
	{
		delete[] m_textureBuffer;
	}
};

#endif