#ifndef SPRITE_H
#define SPRITE_H

#include<memory>
#include"../Core/Effects.h"
#include"Texture.h"

struct Sprite final
{
	void InitializeSprite(const std::shared_ptr<Texture>& textureRef, const bool& useAlphaChannels, const unsigned int& effects)
	{
		m_textureRef = textureRef;
		m_useAlphaChannels = useAlphaChannels;
		m_effects = effects;
	}



	bool m_useAlphaChannels;
	std::shared_ptr<Texture> m_textureRef;
	unsigned int m_effects;
};

#endif
