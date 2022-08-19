#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H


#include"../Base/BaseComponent.h"
#include"../Core/Effects.h"
#include"../GeneralObjects/Texture.h"
#include"../GeneralObjects/Sprite.h"
#include"../GeneralObjects/Rect.h"

struct SpriteComponent final : public BaseComponent
{
	Sprite m_sprite;
};
#endif