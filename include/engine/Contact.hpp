#ifndef EG_CONTACT_HPP
#define EG_CONTACT_HPP


#include <memory>
#include <utility>

#include "ecs/EntityManager.hpp"

#include "utility/Vector2.hpp"

class Contact
{
public:
	Contact(Vector2f position1,
	        Vector2f position2,
			const ecs::Collidable& collidable1,
	        const ecs::Collidable& collidable2);

private:
	void defineForSpheres(Vector2f position1,
	                      Vector2f position2,
	                      const ecs::Collidable& sphere1,
	                      const ecs::Collidable& sphere2);

	void defineForRects(Vector2f position1,
	                    Vector2f position2,
	                    const ecs::Collidable& rect1,
	                    const ecs::Collidable& rect2);

	void defineForMixed(Vector2f spherePos,
	                    Vector2f rectPos,
	                    const ecs::Collidable& sphere,
	                    const ecs::Collidable& rect);

public:
	Vector2f normal_;
	float distance_;
	bool defined_;
};


#endif // EG_CONTACT_HPP
