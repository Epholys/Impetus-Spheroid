//-----------------------------------------------------------------------------
// *** Constructors: ***
template<typename T>
Vector2<T>::Vector2()
	: sf::Vector2<T>()
{
}

template<typename T>
Vector2<T>::Vector2(T X, T Y)
	: sf::Vector2<T>(X, Y)
{
}

template<typename T>
template<typename U>
Vector2<T>::Vector2(const Vector2<U>& vector)
	: sf::Vector2<T>(vector)
{
}


//-----------------------------------------------------------------------------
// *** Maths operation: ***

template<typename T>
T Vector2<T>::dotProduct(const Vector2<T>& vector) const
{
	return sf::Vector2<T>::x * vector.x + sf::Vector2<T>::y * vector.y;
}

template<typename T>
void Vector2<T>::normalize()
{  
	T x = sf::Vector2<T>::x;
	T y = sf::Vector2<T>::y;
		
	float norm = std::sqrt(x*x + y*y);

	sf::Vector2<T>::x /= norm;
	sf::Vector2<T>::y /= norm;
}


//-----------------------------------------------------------------------------
// *** Operator Overloading: ***


template <typename T>
Vector2<T> operator -(const Vector2<T>& right)
{
	return Vector2<T>(-right.Vector2<T>::x, -right.Vector2<T>::y);
}

template <typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
{
	left.Vector2<T>::x += right.Vector2<T>::x;
	left.Vector2<T>::y += right.Vector2<T>::y;
	return left;
}

template <typename T>
Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
{
	left.Vector2<T>::x -= right.Vector2<T>::x;
	left.Vector2<T>::y -= right.Vector2<T>::y;
	return left;
}

template <typename T>
Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.Vector2<T>::x + right.Vector2<T>::x, left.Vector2<T>::y + right.Vector2<T>::y);
}

template <typename T>
Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.Vector2<T>::x - right.Vector2<T>::x, left.Vector2<T>::y - right.Vector2<T>::y);
}

template <typename T>
Vector2<T> operator *(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.Vector2<T>::x * right, left.Vector2<T>::y * right);
}

template <typename T>
Vector2<T> operator *(T left, const Vector2<T>& right)
{
	return Vector2<T>(right.Vector2<T>::x * left, right.Vector2<T>::y * left);
}

template <typename T>
Vector2<T>& operator *=(Vector2<T>& left, T right)
{
	left.Vector2<T>::x *= right;
	left.Vector2<T>::y *= right;
	return left;
}

template <typename T>
Vector2<T> operator /(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.Vector2<T>::x / right, left.Vector2<T>::y / right);
}

template <typename T>
Vector2<T>& operator /=(Vector2<T>& left, T right)
{
	left.Vector2<T>::x /= right;
	left.Vector2<T>::y /= right;
	return left;
}
