#ifndef RECT_H
#define RECT_H

#include"../DataStructures/Vector.h"
class Renderer;

#include<HAPI_Types.h>
using namespace HAPISPACE;


template<class T>
struct Rect final
{
	explicit Rect(const T& x1, const T& y1, const T x2, const T& y2, const Vector2<T>& offsetArg = Vector2<T>(0, 0))
		: x1(x1), x2(x2), y1(y1), y2(y2), m_offset(offsetArg) {};

	Rect() = default;
	~Rect() = default;

	template<class OT>
	Rect<OT> ToType() const;

	const bool IsRectInside(const Rect<T>& otherRect)const;
	const bool IsRectIntersecting(const Rect<T>& otherRect)const;

	const bool IsPointLyingOnRect(const Vector2<T>& point)const;

	void ClampRect(const Rect<T>& otherRect);
	void Translate(const Vector2<T>& position);
	T GetRectHeight()const;
	T GetRectWidth()const;
	Vector2<T> GetRectCentre()const;
	Vector2<T> GetPosition()const;

	void DebugDrawRect(const Vector2i& position, const HAPI_TColour& color, const Renderer& renderer, const bool& useAlphaChannel, const bool& useMonochrome);

	T
		x1{},
		x2{},
		y1{}, 
		y2{};

	Vector2<T> m_offset;

	HAPI_TColour 
		m_debugRectColor{HAPI_TColour::WHITE};
};



template<class T>
template<class OT>
inline Rect<OT> Rect<T>::ToType()const
{
	return Rect<OT>(static_cast<OT>(x1), static_cast<OT>(y1), static_cast<OT>(x2), static_cast<OT>(y2));
}

template<class T>
inline const bool Rect<T>::IsRectIntersecting(const Rect<T>& otherRect) const
{
	return
		x1 < otherRect.x2 &&
		x2 > otherRect.x1 &&
		y1 < otherRect.y2 &&
		y2 > otherRect.y1;
}

template<class T>
inline const bool Rect<T>::IsPointLyingOnRect(const Vector2<T>& point)const
{
	//if points x value lies on x axis of rect and points y value lies on y axis of rect
	return (point.x >= x1 && point.x <= x2 && point.y >= y1 && point.y <= y2);

}

template<class T>
inline void Rect<T>::Translate(const Vector2<T>& position)
{
	Vector2<T> vectorToPosition(position.x - x1, position.y - y1);
	x1 += vectorToPosition.x;
	y1 += vectorToPosition.y;
	x2 += vectorToPosition.x;
	y2 += vectorToPosition.y;
}

template<class T>
inline const bool Rect<T>::IsRectInside(const Rect<T>& otherRect) const
{

	return
		otherRect.x1 >= x1 && otherRect.x2 <= x2 &&
		otherRect.y1 >= y1 && otherRect.y2 <= y2;
}

template<class T>
inline T Rect<T>::GetRectHeight() const
{
	return y2 - y1;
}

template<class T>
inline T Rect<T>::GetRectWidth() const
{
	return x2 - x1;
}

template<class T>
inline Vector2<T> Rect<T>::GetRectCentre() const
{
	return Vector2<T>(this->x1 + this->GetRectWidth()/2, this->y1 + this->GetRectHeight()/2);
}

template<class T>
inline Vector2<T> Rect<T>::GetPosition() const
{
	return Vector2<T>(x1, y1);
}

template<class T>
inline void Rect<T>::DebugDrawRect(const Vector2i& position, const HAPI_TColour& color, const Renderer& renderer, const bool& useAlphaChannel, const bool& useMonochrome)
{
	Vector2i preRectPosition{ GetPosition().ToType<int>() };
	Translate(position.ToType<T>());
	renderer.DrawRectangle(this->ToType<int>(), color, useAlphaChannel, useMonochrome);
	Translate(preRectPosition.ToType<T>());
}

template<class T>
inline void Rect<T>::ClampRect(const Rect<T>& otherRect)
{
	x1 = Utils::ClampValue(x1, otherRect.x1, otherRect.x2);
	y1 = Utils::ClampValue(y1, otherRect.y1, otherRect.y2);
	x2 = Utils::ClampValue(x2, otherRect.x1, otherRect.x2);
	y2 = Utils::ClampValue(y2, otherRect.y1, otherRect.y2);
}

using IntRect = Rect<int>;
using FRect = Rect<float>;


#endif