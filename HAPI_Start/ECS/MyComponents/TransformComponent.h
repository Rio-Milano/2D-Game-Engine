#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include"../Base/BaseComponent.h"
#include"../DataStructures/Vector.h"
#include"../DataStructures/Vector.h"


struct TransformComponent final : public BaseComponent
{

	void InitializeTransformComponent(const Vector2f& positionArg = Vector2f(0.0f, 0.0f), const float& rotationArg = 0, const float& scaleArg = 1)
	{
		position = positionArg;
		rotation = rotationArg;
		scale = scaleArg;
	};

	Vector2f position{ 0, 0 };

	float rotation{ 0.0f };

	float scale{ 1.0f };
};

#endif