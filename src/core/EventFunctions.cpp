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
	/* A modifier's main function which pause velocity for every ball in the
	 * rectangle defined bu its upper left corner POSITION and it size SIZE
	 * */
	auto stopTimeBall =
		[](Vector2f position, Vector2f size, Entity& ball, Time)
	{
		const float PAUSE_DURATION = 5.f;

		if(ball.getType() != EntityID::Ball) return;

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
			velocityComponent->pause(seconds(PAUSE_DURATION));
			if(projectileComponent)
			{
				projectileComponent->pause(seconds(PAUSE_DURATION));
			}
		}

			
	};


	auto stopTimeWorld =
		[](World& world, Time)
	{
		auto windowSize = world.getWindowSize();

		const float LEFT_MARGIN = windowSize.x * 0.25f;
		
		Vector2f zoneUpperLeftCorner (LEFT_MARGIN, 0.f);
		Vector2f zoneSize (windowSize.x - LEFT_MARGIN, windowSize.y);

		Modifier<Entity> stopTimeMod;
		stopTimeMod.preFunction_ = std::bind(stopTimeBall,
		                                    zoneUpperLeftCorner,
		                                    zoneSize,
		                                    std::placeholders::_1,
		                                    std::placeholders::_2);
		stopTimeMod.duration_ = Time();

		world.addEntityModifier(stopTimeMod);
	};

//-----------------------------------------------------------------------------


	auto chgGravUpWorld =
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

			world.getGravityVect().y *= -1.f;
		};


//-----------------------------------------------------------------------------

	void generateAnObstacle (Vector2f upLeftCorner, Vector2f zoneSize,
	                         World& world, Time)
	{
		const float MEAN_HEIGHT = 75.f;
		const float DVT_HEIGHT = 25.f;
		const float MEAN_VEL = -500.f;
		const float DVT_VEL = 150.f;
		const float MEAN_GRAV = 250.f;
		const float DVT_GRAV = 50.f;
		const sf::Color COLOR = sf::Color::Green;

		Vector2f size (15.f, normalRandFloat(MEAN_HEIGHT, DVT_HEIGHT));
		
		float xMean = upLeftCorner.x + zoneSize.x / 2.f;
		float xDev = zoneSize.x / 3.f;
		float xPosition = normalRandFloat(xMean, xDev);
		float yPosition = upLeftCorner.y + zoneSize.y + size.y/2.f;

		float yVelocity = normalRandFloat(MEAN_VEL, DVT_VEL);
		float yGrav = normalRandFloat(MEAN_GRAV, DVT_GRAV);

 		world.addEntity<Obstacle>(Vector2f(xPosition, yPosition),
		                          size,
		                          Vector2f(0.f, yVelocity),
		                          Vector2f(0.f, yGrav),
		                          COLOR);
}

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


	auto addWindWorld = 
		[](World& world, Time)
		{
			world.getGravityVect().x = -750.f;
		};

	auto removeWindWorld = 
		[](World& world, Time)
		{
			world.getGravityVect().x = 0.f;
		};


//-----------------------------------------------------------------------------


	std::vector<Event> generateEvents()
	{
		// Create base modifiers
		Modifier<World> stopTimeMod;
		stopTimeMod.preFunction_ = stopTimeWorld;
		stopTimeMod.duration_ = seconds(5);

		Modifier<World> chgGravUpWorldMod;
		chgGravUpWorldMod.preFunction_ = chgGravUpWorld;
		chgGravUpWorldMod.postFunction_ = chgGravUpWorld;
		chgGravUpWorldMod.duration_ = seconds(5);

		Modifier<World> generateObstaclesMod;
		generateObstaclesMod.preFunction_ = createObstacleModifiers;
		generateObstaclesMod.duration_ = seconds(5);

		Modifier<World> addWindWorldMod;
		addWindWorldMod.preFunction_ = addWindWorld;
		addWindWorldMod.postFunction_ = removeWindWorld;
		addWindWorldMod.duration_ = seconds(5);


		// Create base Events
		Event stopTimeEvt;
		stopTimeEvt.diff = Event::Medium;
		stopTimeEvt.worldModifiers.push_back(stopTimeMod);

		Event chgGravUpWorldEvt;
		chgGravUpWorldEvt.diff = Event::Medium;
		chgGravUpWorldEvt.worldModifiers.push_back(chgGravUpWorldMod);

		Event createObstacleWorldEvt;
		createObstacleWorldEvt.diff = Event::Medium;
		createObstacleWorldEvt.worldModifiers.push_back(generateObstaclesMod);

		Event addWindWorldEvt;
		addWindWorldEvt.chance = 5;
		addWindWorldEvt.diff = Event::Easy;
		addWindWorldEvt.worldModifiers.push_back(addWindWorldMod);


		// Modify Base Modifiers to create more complex Events
		stopTimeMod.preDelay_ = seconds(5.5f);
		Event gravAndTimeEvt;
		gravAndTimeEvt.diff = Event::Hard;
		gravAndTimeEvt.worldModifiers.push_back(chgGravUpWorldMod);
		gravAndTimeEvt.worldModifiers.push_back(stopTimeMod);

		
		// Create and return all the Events
		std::vector<Event> events
			{stopTimeEvt, chgGravUpWorldEvt, createObstacleWorldEvt,
			addWindWorldEvt, gravAndTimeEvt};
		return events;
	}
}
