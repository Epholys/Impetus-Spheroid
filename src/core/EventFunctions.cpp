#include <functional>

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

		if(!velocityComponent || !positionComponent) return;

		
		auto ballPosition = positionComponent->position_;
		
		if(ballPosition.x > position.x &&
		   ballPosition.x < position.x + size.x &&
		   ballPosition.y > position.y &&
		   ballPosition.y < position.y + size.y)
		{
			velocityComponent->pause(seconds(PAUSE_DURATION));
		}
	};


	auto stopTimeWorld =
		[](World& world, Time)
	{
		auto windowSize = world.getWindowSize();

		const float leftMargin = windowSize.x * 0.25f;
		
		Vector2f zoneUpperLeftCorner (leftMargin, 0.f);
		Vector2f zoneSize (windowSize.x - leftMargin, windowSize.y);

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


	auto createObstacleWorld =
		[](World& world, Time)
		{
			
		};


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

		Modifier<World> createObstacleWorldMod;
		createObstacleWorldMod.mainFunction_ = createObstacleWorld;

		Modifier<World> addWindWorldMod;
		addWindWorldMod.preFunction_ = addWindWorld;
		addWindWorldMod.postFunction_ = removeWindWorld;
		addWindWorldMod.duration_ = seconds(5);


		// Create base Events
		Event stopTimeEvt;
		stopTimeEvt.chance = 5;
		stopTimeEvt.worldModifiers.push_back(stopTimeMod);

		Event chgGravUpWorldEvt;
		chgGravUpWorldEvt.chance = 5;
		chgGravUpWorldEvt.worldModifiers.push_back(chgGravUpWorldMod);

		Event createObstacleWorldEvt;
		createObstacleWorldEvt.chance = 0;
		createObstacleWorldEvt.worldModifiers.push_back(createObstacleWorldMod);

		Event addWindWorldEvt;
		addWindWorldEvt.chance = 5;
		addWindWorldEvt.worldModifiers.push_back(addWindWorldMod);


		// Modify Base Modifiers to create more complex Events
		stopTimeMod.preDelay_ = seconds(5.5f);
		Event gravAndTimeEvt;
		gravAndTimeEvt.chance = 2;
		gravAndTimeEvt.worldModifiers.push_back(chgGravUpWorldMod);
		gravAndTimeEvt.worldModifiers.push_back(stopTimeMod);

		
		// Create and return all the Events
		std::vector<Event> events
				{stopTimeEvt, chgGravUpWorldEvt, createObstacleWorldEvt,
				addWindWorldEvt, gravAndTimeEvt};
		return events;
	}
}
