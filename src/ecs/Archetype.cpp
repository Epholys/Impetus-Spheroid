#include <iostream>

#include "ecs/Archetype.hpp"



namespace ecs
{

	Entity createBall(EntityManager& ecs,
	                  Vector2f position,
	                  float radius,
	                  float mass,
	                  Vector2f gravVect,
	                  int points,
	                  int nTouching)
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
		ecs.addComponent<Projectile>(ball, points, nTouching);

		Collidable* pCollSphere = new Collidable(Collidable::Sphere);
		pCollSphere->radius = radius;
		ComponentBase::SPtr spCollSphere (pCollSphere);
		ecs.addComponent(ball, spCollSphere);

		return ball;
	}

	
	Entity createWall(EntityManager& ecs,
	                  Vector2f position,
	                  Vector2f size)
	{
		Entity rect = ecs.addEntity();
		
		ecs.addComponent<Position>(rect, position);
		ecs.addComponent<Solid>(rect, 0.f, 1.f);

		Collidable* pCollRect = new Collidable(Collidable::Rectangle);
		pCollRect->size = size;
		ComponentBase::SPtr spCollRect (pCollRect);
		ecs.addComponent(rect, spCollRect);

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
		ecs.addComponent<Target>(targ, multiplier);

		Collidable* pCollRect = new Collidable(Collidable::Rectangle);
		pCollRect->size = size;
		ComponentBase::SPtr spCollRect (pCollRect);
		ecs.addComponent(targ, spCollRect);

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

		Collidable* pCollRect = new Collidable(Collidable::Rectangle);
		pCollRect->size = size;
		ComponentBase::SPtr spCollRect (pCollRect);
		ecs.addComponent(obst, spCollRect);

		return obst;
	}


} // namespace ecs
