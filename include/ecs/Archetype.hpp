#ifndef ECS_ARCHETYPE_HPP
#define ECS_ARCHETYPE_HPP


#include "utility/Vector2.hpp"
#include "ecs/EntityManager.hpp"

namespace ecs
{

	/* Create a simple ball Object in ecs and return its Entity
	 *
	 * Pre-condition: mass must not be empty. Else, it will automatically
	 * attribute a mass of 1.
	 * 
	 * */
	Entity createBall(EntityManager& ecs,
	                  Vector2f position,
	                  float radius,
	                  float mass,
	                  Vector2f gravVect,
	                  int points = 1,
	                  int nTouching = 1);
	
	Entity createWall(EntityManager& ecs,
	                  Vector2f position,
	                  Vector2f size);

	Entity createTarget(EntityManager& ecs,
	                    Vector2f position,
	                    Vector2f size,
	                    float multiplier = 1.f);

	Entity createObstacle(EntityManager& ecs,
	                      Vector2f position,
	                      Vector2f size,
	                      Vector2f velocity,
	                      float mass,
	                      Vector2f gravVect);

	                      

} // namespace ecs


#endif // ECS_ARCHETYPE_HPP
