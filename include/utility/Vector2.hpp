#ifndef UT_VECTOR2_HPP
#define UT_VECTOR2_HPP


#include <cmath>

#include <SFML/System/Vector2.hpp>


/* A custom vector, implemented from sf::Vector2, adding the dot product.
 * 
 * This class should be as library-independent as possible. However, sf::Vector2
 * is minimalist and can be simply replaced by an other implementation.
 * For more information, see http://www.sfml-dev.org/index.php
 *
 * TODO: Make this class entirely library-independent.
 * */

template<typename T>
class Vector2 : public sf::Vector2<T>
{
public:
	Vector2();
	Vector2(T X, T Y);

	template<typename U>
	explicit Vector2(const Vector2<U>& vector);

	template<typename U>
	explicit Vector2(const sf::Vector2<U>& vector);

	/* Returns the dot product of this and vector */
	T dotProduct(const Vector2& vector) const;

	/* Normalize this vector.
	 * Returns true if this operation succeeded, i.e. *this isn't the null
	 * vector.
	 * */
	bool normalize();
};
	

template <typename T>
Vector2<T> operator -(const Vector2<T>& right);

template <typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
Vector2<T> operator *(const Vector2<T>& left, T right);

template <typename T>
Vector2<T> operator *(T left, const Vector2<T>& right);

template <typename T>
Vector2<T>& operator *=(Vector2<T>& left, T right);

template <typename T>
Vector2<T> operator /(const Vector2<T>& left, T right);

template <typename T>
Vector2<T>& operator /=(Vector2<T>& left, T right);


#include "utility/Vector2.ipp"

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<unsigned int> Vector2u;
	

#endif // UT_VECTOR2_HPP
