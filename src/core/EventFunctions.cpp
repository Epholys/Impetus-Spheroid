#include <functional>

#include "core/EventFunctions.hpp"
#include "core/World.hpp"
#include "core/Entity.hpp"

namespace evt
{
	auto stopTime =
		[](World& world, Time)
		{
			world.getEntityManager().pauseAllComponents(ecs::Component::Velocity, seconds(5));
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
			world.getGravityVect().x = -500.f;
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
		stopTimeMod.preFunction_ = stopTime;
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
