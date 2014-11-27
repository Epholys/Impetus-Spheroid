#include <iostream>

#include "ecs/Archetype.hpp"



namespace ecs
{

	Entity createBall(EntityManager& ecs,
	                  Vector2f position,
	                  float radius,
	                  float mass,
	                  Vector2f gravVect)
	{
		Entity ball = ecs.addEntity();

		if (mass == 0.f)
		{
			mass = 1.f;
		}

		ecs.addComponent<Position>(ball, position);
		ecs.addComponent<Velocity>(ball);
		ecs.addComponent<Mass>(ball, mass, gravVect);
		ecs.addComponent<Solid>(ball, 1.f / mass, 2.f);
		ecs.addComponent<CollidableSphere>(ball, radius);
		ecs.addComponent<Projectile>(ball);

		return ball;
	}

	
	Entity createWall(EntityManager& ecs,
	                  Vector2f position,
	                  Vector2f size)
	{
		Entity rect = ecs.addEntity();
		
		ecs.addComponent<Position>(rect, position);
		ecs.addComponent<Solid>(rect, 0.f, 1.f);
		ecs.addComponent<CollidableRect>(rect, size);

		return rect;
	}

	Entity createTarget(EntityManager& ecs,
	                    Vector2f position,
	                    Vector2f size)
	{
		Entity targ = ecs.addEntity();

		ecs.addComponent<Position>(targ, position);
		ecs.addComponent<Velocity>(targ);
		ecs.addComponent<Solid>(targ, 0.f, 1.f);
		ecs.addComponent<CollidableRect>(targ, size);
		ecs.addComponent<Target>(targ);

		return targ;
	}


} // namespace ecs
