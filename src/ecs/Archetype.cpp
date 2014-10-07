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
	ecs.addComponent<Mass>(ball, mass);
	ecs.addComponent<Solid>(ball, 1.f / mass, 1.f);
	ecs.addComponent<CollidableSphere>(ball, radius);
	ecs.addComponent<Projectile>(ball);

	return ball;
}

} // namespace ecs
