//-----------------------------------------------------------------------------
// *** Constructors: ***
template<typename T>
Vector2<T>::Vector2<T>()
	: sf::Vector2<T>()
{
}

template<typename T>
Vector2<T>::Vector2<T>(T X, T Y)
	: sf::Vector2<T>(X, Y)
{
}

template<typename T>
template<typename U>
Vector2<T>::Vector2<T>(const Vector2<U>& vector)
	: sf::Vector2<T>(vector)
{
}


//-----------------------------------------------------------------------------
// *** Maths operation: ***

template<typename T>
T Vector2<T>::dotProduct(const Vector2<T>& vector) const
{
	return x * vector.x + y * vector.y;
}
