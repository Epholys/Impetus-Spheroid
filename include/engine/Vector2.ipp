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
