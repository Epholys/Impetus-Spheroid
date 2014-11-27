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
	
	void moveEntityTo(int xPos, int yPos, ecs::Entity label, Entity& entity, Time)
	{
		if(entity.getLabel() == label)
		{
			auto velComp =
				dynCast<ecs::Velocity>(entity.getComponents()[ecs::Component::Velocity]);
			auto posComp =
				dynCast<ecs::Position>(entity.getComponents()[ecs::Component::Position]);

			if(velComp && velComp)
			{
				Vector2f vectToObjective = Vector2f(xPos, yPos) - posComp->position_;
				velComp->velocity_ = velComp->velocity_ * 0.25f + vectToObjective * 2.5f;
			}
		}
	}
	
	void resetEntityVelocity(ecs::Entity label, Entity& entity, Time)
	{
		if(entity.getLabel() == label)
		{
			auto velComp =
				dynCast<ecs::Velocity>(entity.getComponents()[ecs::Component::Velocity]);
			if(velComp)
			{
				velComp->velocity_ = Vector2f(0.f, 0.f);
			}
		}
	}

	auto teleportTarget =
		[](World& world, Time)
		{
			auto collisions = world.getPhysicEngine().getTrackedCollisions();
			
			for(auto& pair : collisions)
			{
				auto posPtr = world.getEntityManager().getComponent(pair.second, ecs::Component::Position);
				auto posComp = dynCast<ecs::Position>(posPtr);
				if(posComp)
				{
					auto windowSize = world.getWindowSize();
					int newXPosition = randInt(windowSize.x / 4, windowSize.x - 20);
					int newYPosition = randInt(80, windowSize.y - 80);

					Modifier<Entity> moveMod;
					moveMod.mainFunction_ = std::bind(moveEntityTo, newXPosition, newYPosition,
					                                 pair.second, std::placeholders::_1,
					                                 std::placeholders::_2);
					moveMod.postFunction_ = std::bind(resetEntityVelocity, pair.second,
					                                  std::placeholders::_1, std::placeholders::_2);
					moveMod.duration_ = seconds(1.5f);
					world.addEntityModifier(moveMod);
				}
			}
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

		Modifier<World> teleportTargetMod;
		teleportTargetMod.mainFunction_ = teleportTarget;
		teleportTargetMod.duration_ = seconds(10);

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

		Event teleportTargetEvt;
		teleportTargetEvt.chance = 100;
		teleportTargetEvt.worldModifiers.push_back(teleportTargetMod);

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
			{stopTimeEvt, teleportTargetEvt, chgGravUpWorldEvt,
			 createObstacleWorldEvt, addWindWorldEvt, gravAndTimeEvt};
		return events;
	}
}
