#ifndef VECTOR_H
#define VECTOR_H

//vector/point representation

template<class T>
struct Vector2 final
{
	Vector2(const T& x, const T& y)
	:x(x), y(y){}

	Vector2() = default;

	template<class OT>
	Vector2<OT> ToType() const;

	static T Distance(const Vector2<T>& A, const Vector2<T>& B);

	Vector2<T> Invert()const;


	T Magnitude() const;
	Vector2<T> Normalize() const;

	Vector2<T> operator-(const Vector2<T>& other)const
	{
		return Vector2<T>(x - other.x, y - other.y);
	}

	bool operator==(const Vector2<T>& other)const
	{
		return (x == other.x &&  y == other.y);
	}

	Vector2<T> operator*(const T& value)const
	{
		return Vector2<T>(x * value, y * value);
	}


	Vector2<T> operator+(const Vector2<T>& other)const
	{
		return Vector2<T>(x + other.x, y + other.y);
	}

	Vector2<T> operator+=(const Vector2<T>& other)const
	{
		return *(this)+other;
	}

	Vector2<T> operator*(const Vector2<T>& other)const
	{
		return Vector2<T>(x * other.x, y * other.y);

	}

	Vector2<T> operator/(const T& value)const
	{
		return Vector2<T>(x/value, y/value);
	}

	Vector2<T> operator/(const Vector2<T>& otherVector)const
	{
		return Vector2<T>(x / otherVector.x, y / otherVector.y);
	}

	bool operator!=(const Vector2<T>& otherVector)const
	{
		return (this->x != otherVector.x || this->y != otherVector.y);
	}
	


	T x, y;
};

template<class T>
inline T Vector2<T>::Magnitude() const
{
	return static_cast<T>(sqrt(pow(x, 2) + pow(y, 2)));
}

template<class T>
inline Vector2<T> Vector2<T>::Normalize() const
{
	T magnitude{ this->Magnitude() };	
	
	//prevent division by 0
	//if (!magnitude)
		//return Vector2<T>(static_cast<T>(0), static_cast<T>(0));

	Vector2<T> normal{ this->x / magnitude, this->y / magnitude };
	return normal;
}
template<class T>
inline T Vector2<T>::Distance(const Vector2<T>& A, const Vector2<T>& B)
{
	Vector2<T> AB{B-A};
	return AB.Magnitude();
}
template<class T>
template<class OT>
inline Vector2<OT> Vector2<T>::ToType() const
{
	return Vector2<OT>(static_cast<OT>(x), static_cast<OT>(y));
}


template<class T>
inline Vector2<T> Vector2<T>::Invert()const
{
	return Vector2<T>(-x, -y);
}

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;









template<class T>
struct Vector3 final
{
	explicit Vector3(const T& x, const T& y, const T& z)
	:x(x), y(y), z(z) {}

	Vector3() = default;

	template <class OT>
	Vector3<OT> ToType()const;

	Vector3<T> Invert()const;

	T Magnitude() const;
	Vector3<T> Normalize() const;

	T x, y, z;
};

template<class T>
inline T Vector3<T>::Magnitude() const
{
	return static_cast<T>(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
}

template<class T>
inline Vector3<T> Vector3<T>::Normalize() const
{
	T magnitude{ this->Magnitude() };
	Vector3<T> normal{ this->x / magnitude, this->y / magnitude, this.z / magnitude };
}
using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;

template<class T>
template<class OT> 
inline Vector3<OT> Vector3<T>::ToType() const
{
	return Vector3<OT>(static_cast<OT>(x), static_cast<OT>(y), static_cast<OT>(z));
}

template<class T>
inline Vector3<T> Vector3<T>::Invert()const
{
	return Vector3<T>(-x, -y, -z);
}

#endif




