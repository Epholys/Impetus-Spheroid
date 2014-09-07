#ifndef EG_VECTOR2_HPP
#define EG_VECTOR2_HPP


#include <SFML/System/Vector2.hpp>

namespace eg
{
	/* A custom vector, for the physic engine, implemented from sf::Vector2.
	 * 
	 * This class should be as library-independent as possible. However,
	 * sf::Vector2 is minimalist and can be simply replaced by an other
	 * implementation.
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

		T dotProduct(const Vector2& vector) const;
	};
	
#include "engine/Vector2.ipp"

	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;
	typedef Vector2<unsigned int> Vector2u;
	
} // namespace eg


#endif // EG_VECTOR2_HPP
