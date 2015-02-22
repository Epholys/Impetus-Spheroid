#include <iostream>

#include "ecs/Archetype.hpp"



namespace ecs
{

	Entity createBall(EntityManager& ecs,
	                  Vector2f position,
	                  float radius,
	                  float mass,
	                  Vector2f gravVect,
	                  int points)
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
		ecs.addComponent<Projectile>(ball, points);

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
	                    Vector2f size,
	                    float multiplier)
	{
		Entity targ = ecs.addEntity();

		ecs.addComponent<Position>(targ, position);
		ecs.addComponent<Velocity>(targ);
		ecs.addComponent<Solid>(targ, 0.f, 1.f);
		ecs.addComponent<CollidableRect>(targ, size);
		ecs.addComponent<Target>(targ, multiplier);

		return targ;
	}


	Entity createObstacle(EntityManager& ecs,
	                      Vector2f position,
	                      Vector2f size,
	                      Vector2f velocity,
	                      float mass,
	                      Vector2f gravVect)
	{
		Entity obst = ecs.addEntity();

		if(mass == 0.f)
		{
			mass = 1.f;
		}

		ecs.addComponent<Position>(obst, position);
		ecs.addComponent<Velocity>(obst, velocity);
		ecs.addComponent<Mass>(obst, mass, gravVect);
		ecs.addComponent<Solid>(obst, 1.f / mass, 1.5f);
		ecs.addComponent<CollidableRect>(obst, size);

		return obst;
	}


} // namespace ecs
