#include <iostream>

#include "ecs/Archetype.hpp"



namespace ecs
{

	Entity createBall(EntityManager& ecs,
	                  Vector2f position,
	                  float radius,
	                  float mass)
	{
		Entity ball = ecs.addEntity();

		if (mass == 0.f)
		{
			mass = 1.f;
		}

		ecs.addComponent<Position>(ball, position);
		ecs.addComponent<Velocity>(ball);
		ecs.addComponent<Mass>(ball, mass, Vector2f(0.f, 1000.f));
		ecs.addComponent<Solid>(ball, 1.f / mass, 2.f);
		ecs.addComponent<CollidableSphere>(ball, radius);
		ecs.addComponent<Projectile>(ball);

		return ball;
	}

	
	Entity createRect(EntityManager& ecs,
	                  Vector2f position,
	                  Vector2f size)
	{
		Entity rect = ecs.addEntity();
		
		ecs.addComponent<Position>(rect, position);
		ecs.addComponent<Solid>(rect, 0.f, 1.f);
		ecs.addComponent<CollidableRect>(rect, size);
		ecs.addComponent<Target>(rect);

		return rect;
	}


} // namespace ecs
