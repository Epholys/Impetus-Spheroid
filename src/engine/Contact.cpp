#include "engine/Contact.hpp"


//-----------------------------------------------------------------------------
// *** constructor: ***

Contact::Contact(Vector2f position1,
                 Vector2f position2,
                 const ecs::Collidable& collidable1,
                 const ecs::Collidable& collidable2)
	: normal_(0.f, 0.f)
	, distance_(-1.f)
	, defined_(false)
{
	if (collidable1.type == ecs::Collidable::None ||
	    collidable2.type == ecs::Collidable::None)
	{
		// Do nothing
	}

	else if(collidable1.type == ecs::Collidable::Sphere &&
	        collidable2.type == ecs::Collidable::Sphere)
	{
		defineForSpheres(position1, position2, collidable1, collidable2);
	}

	else if(collidable1.type == ecs::Collidable::Rectangle &&
	        collidable2.type == ecs::Collidable::Rectangle)
	{
		defineForRects(position1, position2, collidable1, collidable2);
	}

	else
	{
		// I tried swapping but I ran into some problems, so here's the ugly
		// version: 
		if(collidable1.type == ecs::Collidable::Sphere)
		{
			defineForMixed(position1, position2, collidable1, collidable2);
			normal_ *= -1.f;
		}
		else
		{
			defineForMixed(position2, position1, collidable2, collidable1);
		}
	}
}


//-----------------------------------------------------------------------------
// *** Contact generation: ***
void Contact::defineForSpheres(Vector2f position1,
                               Vector2f position2,
                               const ecs::Collidable& sphere1,
                               const ecs::Collidable& sphere2)
{
	normal_ = Vector2f(position2.x - position1.x,
	                   position2.y - position1.y);

	distance_ = (std::sqrt(std::pow(normal_.x, 2)
	                       + std::pow(normal_.y, 2))
	             - sphere1.radius
	             - sphere2.radius);

	defined_ = true;
}


void Contact::defineForRects(Vector2f position1,
                             Vector2f position2,
                             const ecs::Collidable& rect1,
                             const ecs::Collidable& rect2)
{
	Vector2f gap = position2 - position1;

	bool insideX = !(std::max(std::abs(gap.x) - rect1.size.x/2.f - rect2.size.x/2.f, 0.f));
	bool insideY = !(std::max(std::abs(gap.y) - rect1.size.y/2.f - rect2.size.y/2.f, 0.f));

	float xFactor = std::signbit(gap.x) ? -1 : 1;
	float yFactor = std::signbit(gap.y) ? -1 : 1;

	gap = Vector2f(gap.x - xFactor * (rect1.size.x/2.f + rect2.size.x/2.f),
	               gap.y - yFactor * (rect1.size.y/2.f + rect2.size.y/2.f));

	if(insideX)
		gap.x = 0.f;
	if(insideY)
		gap.y = 0.f;
		
	normal_ = gap;

	distance_ = std::sqrt(std::pow(gap.x, 2) + std::pow(gap.y, 2));

	defined_ = true;
}


void Contact::defineForMixed(Vector2f spherePos,
                             Vector2f rectPos,
                             const ecs::Collidable& sphere,
                             const ecs::Collidable& rect)
{
	Vector2f gap (spherePos.x - rectPos.x, spherePos.y - rectPos.y);

	bool insideX = !(std::max(std::abs(gap.x) - rect.size.x / 2.f, 0.f));
	bool insideY = !(std::max(std::abs(gap.y) - rect.size.y / 2.f, 0.f));

	float xFactor = std::signbit(gap.x) ? -1 : 1;
	float yFactor = std::signbit(gap.y) ? -1 : 1;

	gap = Vector2f(gap.x - xFactor * rect.size.x / 2.f,
	               gap.y - yFactor * rect.size.y / 2.f);

	if(insideX)
		gap.x = 0.f;
	if(insideY)
		gap.y = 0.f;
		
	normal_ = gap;

	distance_ = (std::sqrt(std::pow(gap.x, 2) + std::pow(gap.y, 2))
	             - sphere.radius);

	defined_ = true;
}
