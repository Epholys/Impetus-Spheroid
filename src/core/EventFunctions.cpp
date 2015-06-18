#include <functional>
#include <vector>

#include <SFML/Graphics/Color.hpp>

#include "ecs/Archetype.hpp"
#include "core/Obstacle.hpp"
#include "core/EventFunctions.hpp"
#include "core/World.hpp"
#include "core/Entity.hpp"

namespace evt
{
	namespace
	{
		const int DEFAULT_EVENT_DURATION = 5;
	}

	/* Pause velocity fort the Ball if it's in the rectangle defined by its
	 * upper left corner at position and its size.
	 * */
	auto stopTimeBall =
		[](Vector2f position, Vector2f size, Time pause, Entity& ball, Time)
	{
		if(ball.getType() != EntityType::Ball) return;

		auto components = ball.getComponents();
		
		auto velocityComponent = dynCast<ecs::Velocity>
			(components[ecs::Component::Velocity]);
		auto positionComponent = dynCast<ecs::Position>
			(components[ecs::Component::Position]);
		auto projectileComponent = dynCast<ecs::Projectile>
			(components[ecs::Component::Projectile]);

		if(!velocityComponent || !positionComponent) return;

		
		auto ballPosition = positionComponent->position_;
		
		if(ballPosition.x > position.x &&
		   ballPosition.x < position.x + size.x &&
		   ballPosition.y > position.y &&
		   ballPosition.y < position.y + size.y)
		{
			velocityComponent->pause(pause);
			if(projectileComponent)
			{
				projectileComponent->pause(pause);
			}
		}
	};

	/* Resume time for a Ball that are stopped mid-air.
	 * */
	auto resumeTimeBall = [](Entity& ball, Time)
	{
		if(ball.getType() != EntityType::Ball) return;

		auto components = ball.getComponents(true);
		
		auto velocityComponent = dynCast<ecs::Velocity>
			(components[ecs::Component::Velocity]);
		auto projectileComponent = dynCast<ecs::Projectile>
			(components[ecs::Component::Projectile]);

		if(!velocityComponent || !projectileComponent) return;
		
		velocityComponent->unpause();
		projectileComponent->unpause();
	};

	
	/* Stop time for all balls entities of World.
	 * */
	auto stopTimeWorld =
		[](Time pause, World& world, Time)
	{
		auto windowSize = world.getWindowSize();

		const float LEFT_MARGIN = windowSize.x * 0.25f;
		
		Vector2f zoneUpperLeftCorner (LEFT_MARGIN, 0.f);
		Vector2f zoneSize (windowSize.x - LEFT_MARGIN, windowSize.y);

		Modifier<Entity> stopTimeMod;
		stopTimeMod.postFunction_ = std::bind(stopTimeBall,
		                                      zoneUpperLeftCorner,
		                                      zoneSize,
		                                      pause,
		                                      std::placeholders::_1,
		                                      std::placeholders::_2);
		stopTimeMod.duration_ = Time();

		world.forwardModifier<Entity>(stopTimeMod);
	};

	/* Resume times for all World's Balls entities.  
	 * */
	auto resumeTimeWorld =
		[](World& world, Time)
	{
		Modifier<Entity> resumeTimeMod;
		resumeTimeMod.postFunction_ = std::bind(resumeTimeBall,
		                                        std::placeholders::_1,
		                                        std::placeholders::_2);
		resumeTimeMod.duration_ = Time();

		world.forwardModifier<Entity>(resumeTimeMod);
	};


//-----------------------------------------------------------------------------

	/* Generate an Obstacle Entity with a random size, velocityand a random
	 * position (within the zone delimited by its upper left-hand corner
	 * upLeftCorner and its size zonSize. THen, add this Obstacle in world.
	 * */
	void generateAnObstacle (Vector2f upLeftCorner, Vector2f zoneSize,
	                         World& world, Time)
	{
		const float WIDTH = 15.f;
		const float MEAN_HEIGHT = 75.f;
		const float DVT_HEIGHT = 25.f;
		const float MEAN_VEL = -500.f;
		const float DVT_VEL = 150.f;
		const float MEAN_GRAV = 250.f;
		const float DVT_GRAV = 50.f;

		Vector2f size (WIDTH, normalRandFloat(MEAN_HEIGHT, DVT_HEIGHT));
		
		float xMean = upLeftCorner.x + zoneSize.x / 2.f;
		float xDev = zoneSize.x / 3.f;
		float xPosition = normalRandFloat(xMean, xDev);
		float yPosition = upLeftCorner.y + zoneSize.y + size.y/2.f;

		float yVelocity = normalRandFloat(MEAN_VEL, DVT_VEL);
		float yGrav = normalRandFloat(MEAN_GRAV, DVT_GRAV);

 		world.addEntity<Obstacle>(Vector2f(xPosition, yPosition),
		                          size,
		                          Vector2f(0.f, yVelocity),
		                          Vector2f(0.f, yGrav));
	}

	/* Generate a random amount of Obstacles at a random frequency.
	 * */ 
	void createObstacleModifiers(World& world, Time)
	{
		auto windowSize = world.getWindowSize();

		const float LEFT_MARGIN = windowSize.x * 0.25f;
		const float RIGHT_MARGIN = windowSize.x * 0.25f;

		Vector2f zoneUpperLeftCorner (LEFT_MARGIN, 0.f);
		Vector2f zoneSize (windowSize.x - LEFT_MARGIN - RIGHT_MARGIN, windowSize.y - 1);
		
		const int MIN_OBSTACLE = 3;
		const int MAX_OBSTACLE = 10;
		const float MEAN_DELAY = 1.f;
		const float DEVIATION = 0.5f;
		
		int nObstacle = randInt(MIN_OBSTACLE, MAX_OBSTACLE);
		float totalDelay = 0.f;
		for(int i=0; i<nObstacle; ++i)
		{
			float delay = normalRandFloat(MEAN_DELAY, DEVIATION);
			delay = std::max(0.f, delay);
			totalDelay += delay;

			Modifier<World> generateObstacle;
			generateObstacle.preDelay_ = seconds(totalDelay);
			generateObstacle.preFunction_ = std::bind(generateAnObstacle,
			                                          zoneUpperLeftCorner,
			                                          zoneSize,
			                                          std::placeholders::_1,
			                                          std::placeholders::_2);
			generateObstacle.duration_ = seconds(delay);
			
			world.addModifier(generateObstacle);
		}
	}


//-----------------------------------------------------------------------------

	/* Reverse the gravity of world for all existing Entities and for new Balls
	 * */
	auto reverseGravWorld =
		[](World& world, Time)
		{
			auto entm = world.getEntityManager();
			auto vComp = entm.getAllComponents(ecs::Component::Mass);
			for(auto& comp : vComp)
			{
				auto mass = dynCast<ecs::Mass>(comp);
				if(mass)
				{
				  	mass->gravityVect_.y *= -1.f;
				}
			}

			auto gravVect = world.getGravityVect();
			gravVect.y *= -1;
			world.setGravityVect(gravVect);
		};


//-----------------------------------------------------------------------------

	/* Add a little wind by changing the gravity.
	 * */
	auto addWindWorld = 
		[](World& world, Time)
		{
			auto gravVec = world.getGravityVect();
			gravVec.x = -750.f;
			world.setGravityVect(gravVec);
		};

	/* Remove the wind.
	 * */
	auto removeWindWorld = 
		[](World& world, Time)
		{
			auto gravVec = world.getGravityVect();
			gravVec.x = 0.f;
			world.setGravityVect(gravVec);
		};


//-----------------------------------------------------------------------------


	std::vector<Event> generateEvents()
	{
		// Create base modifiers
		Modifier<World> stopTimeMod;
		stopTimeMod.duration_ = seconds(DEFAULT_EVENT_DURATION);
		stopTimeMod.preFunction_ = std::bind(stopTimeWorld,
		                                     stopTimeMod.duration_,
		                                     std::placeholders::_1,
		                                     std::placeholders::_2);
		stopTimeMod.postFunction_ = resumeTimeWorld;

		Modifier<World> reverseGravWorldMod;
		reverseGravWorldMod.duration_ = seconds(DEFAULT_EVENT_DURATION);
		reverseGravWorldMod.preFunction_ = reverseGravWorld;
		reverseGravWorldMod.postFunction_ = reverseGravWorld;

		Modifier<World> generateObstaclesMod;
		generateObstaclesMod.duration_ = seconds(DEFAULT_EVENT_DURATION);
		generateObstaclesMod.preFunction_ = createObstacleModifiers;

		Modifier<World> addWindWorldMod;
		addWindWorldMod.duration_ = seconds(DEFAULT_EVENT_DURATION);
		addWindWorldMod.preFunction_ = addWindWorld;
		addWindWorldMod.postFunction_ = removeWindWorld;


		// Create base Events
		Event stopTimeEvt;
		stopTimeEvt.diff = Event::Medium;
		stopTimeEvt.worldModifiers.push_back(stopTimeMod);

		Event reverseGravWorldEvt;
		reverseGravWorldEvt.diff = Event::Medium;
		reverseGravWorldEvt.worldModifiers.push_back(reverseGravWorldMod);

		Event createObstacleWorldEvt;
		createObstacleWorldEvt.diff = Event::Medium;
		createObstacleWorldEvt.worldModifiers.push_back(generateObstaclesMod);

		Event addWindWorldEvt;
		addWindWorldEvt.diff = Event::Easy;
		addWindWorldEvt.worldModifiers.push_back(addWindWorldMod);


		// Modify Base Modifiers to create more complex Events
		const float BALL_FALLING = 0.5f;
		const float PRE_DELAY = DEFAULT_EVENT_DURATION + BALL_FALLING;
		stopTimeMod.preDelay_ = seconds(PRE_DELAY);
		Event gravAndTimeEvt;
		gravAndTimeEvt.diff = Event::Hard;
		gravAndTimeEvt.worldModifiers.push_back(reverseGravWorldMod);
		gravAndTimeEvt.worldModifiers.push_back(stopTimeMod);

		
		// Create and return all the Events
		std::vector<Event> events
			{reverseGravWorldEvt, stopTimeEvt, createObstacleWorldEvt,
			addWindWorldEvt, gravAndTimeEvt};
		return events;
	}
}
