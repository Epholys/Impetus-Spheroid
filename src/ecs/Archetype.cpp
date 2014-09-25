#include <iostream>

#include "ecs/Archetype.hpp"



namespace ecs
{

Entity createBall(EntityManager& ecs, Vector2f position, float radius)
{
	Entity ball = ecs.addEntity();

	ecs.addComponent<Position>(ball, position);
	ecs.addComponent<Velocity>(ball);
	ecs.addComponent<Mass>(ball);
	ecs.addComponent<Solid>(ball, 1.f);
	ecs.addComponent<CollidableSphere>(ball, radius);
	ecs.addComponent<Projectile>(ball);

	return ball;
}

} // namespace ecs
