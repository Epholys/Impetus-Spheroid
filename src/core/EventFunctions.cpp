#include <functional>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "framework/Assertion.hpp"
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
		[](sf::FloatRect noBallZone, Time pause, Entity& ball, Time)
	{
		if(ball.getType() != EntityType::Ball) return;

		auto components = ball.getComponents();
		
		auto positionComponent = dynCast<ecs::Position>
			(components[ecs::Component::Position]);
		auto timeComponent = dynCast<ecs::TimeArrow>
			(components[ecs::Component::TimeArrow]);
		

		if(!positionComponent | !timeComponent) return;

		
		auto ballPosition = positionComponent->position_;
		
		if(noBallZone.contains(ballPosition))
			return;

		
		auto velocityComponent = dynCast<ecs::Velocity>
			(components[ecs::Component::Velocity]);
		auto projectileComponent = dynCast<ecs::Projectile>
			(components[ecs::Component::Projectile]);
		
		Time SLOWMO_DURATION = seconds(DEFAULT_EVENT_DURATION / 20.f);
		float timeCoeff = timeComponent->timeCoefficient_;
		float lowerTimeCoeff = (- timeCoeff) / SLOWMO_DURATION.asSeconds();

		Modifier<Entity> stopEnt;
		stopEnt.duration_ = SLOWMO_DURATION;

		stopEnt.mainFunction_ =
		[lowerTimeCoeff, timeComponent](Entity&, Time dt)
		{ timeComponent->timeCoefficient_ += lowerTimeCoeff * dt.asSeconds(); };

		stopEnt.postFunction_ =
		[timeComponent, projectileComponent, velocityComponent, pause, SLOWMO_DURATION, timeCoeff](Entity&, Time)
		{
			if(projectileComponent)
			{
				// Add some margin, as it will be manually unpaused
				projectileComponent->pause(pause + SLOWMO_DURATION*2.f + seconds(1.f));
			}
			if(velocityComponent)
			{
				velocityComponent->pause(pause + SLOWMO_DURATION*2.f + seconds(1.f));
			}
				
			timeComponent->timeCoefficient_ = 0.f;
		};

		ball.addModifier(stopEnt);
	};

	/* Resume time for a Ball that are stopped mid-air.
	 * */
	auto resumeTimeBall = [](Entity& ball, Time)
	{
		if(ball.getType() != EntityType::Ball) return;

		auto components = ball.getComponents(true);
		
		auto projectileComponent = dynCast<ecs::Projectile>
			(components[ecs::Component::Projectile]);
		auto timeComponent = dynCast<ecs::TimeArrow>
			(components[ecs::Component::TimeArrow]);
		auto velocityComponent = dynCast<ecs::Velocity>
			(components[ecs::Component::Velocity]);
		
		if(!timeComponent ||
		   std::abs(timeComponent->timeCoefficient_) > 0.0001f)
			return;

		Time SLOWMO_DURATION = seconds(DEFAULT_EVENT_DURATION / 20.f);
		float higherTimeCoeff = (1.f) / SLOWMO_DURATION.asSeconds();
		
		Modifier<Entity> resumeEnt;
		resumeEnt.duration_ = SLOWMO_DURATION;

		resumeEnt.preFunction_ =
		[timeComponent, projectileComponent, velocityComponent](Entity&, Time)
		{
			if(projectileComponent)
				projectileComponent->unpause();
			if(velocityComponent)
				velocityComponent->unpause();
		};
		
		resumeEnt.mainFunction_ =
		[timeComponent, higherTimeCoeff](Entity&, Time dt)
		{ timeComponent->timeCoefficient_ += higherTimeCoeff * dt.asSeconds(); };
		  
		resumeEnt.postFunction_ =
		[timeComponent](Entity&, Time)
		{ timeComponent->timeCoefficient_ = 1.f; };

		ball.addModifier(resumeEnt);
	};

	
	/* Stop time for all balls entities of World.
	 * */
	auto stopTimeWorld =
		[](Time pause, World& world, Time)
	{
		auto windowSize = world.getWindowSize();

		sf::FloatRect noBallZone (0.f, windowSize.y - 100.f, 100.f, 100.f);
		
		Modifier<Entity> stopTimeMod;
		stopTimeMod.postFunction_ = std::bind(stopTimeBall,
		                                      noBallZone,
		                                      pause,
		                                      std::placeholders::_1,
		                                      std::placeholders::_2);
		stopTimeMod.duration_ = Time();

		world.forwardModifier<Entity>(stopTimeMod);

		bool fadeOut = true, isInFront = true;
		gui::Transition transition (nullptr,
		                            gui::Transition::Linear,
		                            gui::TransformData(Vector2f(world.getWindowSize() / 2u),
		                                               0.f,
		                                               Vector2f(0.75f, 0.75f)),
		                            gui::TransformData(Vector2f(world.getWindowSize() / 2u),
		                                               0.f,
		                                               Vector2f(0.25f, 0.25f)),
		                            seconds(0.5f));
		gui::MultiTransition transitions;
		transitions.addTransition(transition);
		world.addSprite(TextureID::Hourglass,
		                "./media/sprites/Hourglass.png",
		                sf::Color(255,255,255,150),
		                transitions,
		                fadeOut,
		                !isInFront);
				                                
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

	auto addArrows =
		[](World& world, unsigned int number, float angle, Vector2f direction, sf::Color color, float sizeCoeff)
	{
		const Time ARROW_DURATION = seconds(1.f);
		
		Vector2u windowSize = world.getWindowSize();
		sf::FloatRect spanSize (windowSize.x / 10,
		                        windowSize.y / 10,
		                        windowSize.x - 2 * windowSize.x / 10,
		                        windowSize.y - 2 * windowSize.y / 10);
		for(unsigned int i=0; i<number; ++i)
		{
			int x = randInt(0, spanSize.width);
			int y = randInt(0, spanSize.height);

			gui::TransformData begin (Vector2f(spanSize.left + x, spanSize.top + y),
			                          angle,
			                          Vector2f(sizeCoeff, sizeCoeff));

			gui::TransformData end (Vector2f(spanSize.left + x, spanSize.top + y) + direction,
			                        angle,
			                        Vector2f(sizeCoeff, sizeCoeff));

			gui::Transition transition (nullptr, gui::Transition::Quadratic, begin, end, ARROW_DURATION);
			gui::MultiTransition transitions;
			transitions.addTransition(transition);
			bool fadeOut = true, isInFront = true;
			world.addSprite(TextureID::Arrow, "./media/sprites/Arrow.png", color, transitions, fadeOut, !isInFront);
		}
	};
	
	auto addParticleWall =
		[](Wall* wall, Vector2f distance, sf::Color color, float MEAN_RATE)
		{
			const int N_EMITTERS = 50;
			const float DEV_RATE = MEAN_RATE / 2.f;
			Vector2f size = wall->getSize();
			Vector2f gap = size / static_cast<float>(N_EMITTERS);
			gap = (distance.x == 0) ? Vector2f{gap.x, 0.f} : gap;
			gap = (distance.y == 0) ? Vector2f{0.f, gap.y} : gap;
			Vector2f basePosition = wall->getPosition() - wall->getSize() / 2.f + distance;
			for(int i=0; i<N_EMITTERS; ++i)
			{
				float emissionRate = std::abs(normalRandFloat(MEAN_RATE, DEV_RATE));
				wall->addParticleEmitter(Particle::Bubble,
				                         basePosition,
				                         emissionRate,
				                         color,
				                         -distance / 2.f);
				basePosition += gap;

			}

			wall->setOutlineColor(color);
		};

	/* Reverse the gravity of world for all existing Entities and for new Balls.
	 * */
	auto reverseGravWorld =
		[](World& world, Time, bool isOver)
		{
			auto& entm = world.getEntityManager();
			auto vComp = entm.getAllComponents(ecs::Component::Mass, true);
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

			if(!isOver)
			{
				addParticleWall(world.getCeiling(), Vector2f(0.f, 40.f), sf::Color::Blue, 0.75f);
				addArrows(world, 10, -90.f, Vector2f(0.f, -150.f), sf::Color::Blue, 0.75f);

				Modifier<Entity> modifier;
				modifier.duration_ = seconds(DEFAULT_EVENT_DURATION);
				modifier.postFunction_ =
					[&](Entity& ent, Time)
					{
						if(&ent != world.getCeiling()) return;
						world.getCeiling()->removeAllParticleEmitters();
						world.getCeiling()->resetOutlineColor();
					};
				world.forwardModifier<Entity>(modifier);
			}
		};


//-----------------------------------------------------------------------------

	/* Add a little wind by changing the gravity.
	 * */
	auto addWindWorld = 
		[](World& world, Time)
		{
			auto& entm = world.getEntityManager();
			auto vComp = entm.getAllComponents(ecs::Component::Mass, true);
			for(auto& comp : vComp)
			{
				auto mass = dynCast<ecs::Mass>(comp);
				if(mass)
				{
				  	mass->gravityVect_.x = -750.f;
				}
			}

			auto gravVec = world.getGravityVect();
			gravVec.x = -750.f;
			world.setGravityVect(gravVec);

			addParticleWall(world.getLeftWall(), Vector2f(60.f, 0.f), sf::Color(0,150,255), 0.25f);
			addArrows(world, 10, 180.f, Vector2f(-50.f, 0.f), sf::Color(0,150,255), 0.5f);
		};

	/* Remove the wind.
	 * */
	auto removeWindWorld = 
		[](World& world, Time)
		{
			auto& entm = world.getEntityManager();
			auto vComp = entm.getAllComponents(ecs::Component::Mass, true);
			for(auto& comp : vComp)
			{
				auto mass = dynCast<ecs::Mass>(comp);
				if(mass)
				{
				  	mass->gravityVect_.x = 0.f;
				}
			}
			
			auto gravVec = world.getGravityVect();
			gravVec.x = 0.f;
			world.setGravityVect(gravVec);

			
			Modifier<Entity> modifier;
			modifier.duration_ = Time();
			modifier.postFunction_ =
			[&](Entity& ent, Time)
			{
				if(&ent != world.getLeftWall()) return;
				world.getLeftWall()->removeAllParticleEmitters();
				world.getLeftWall()->resetOutlineColor();
			};
			world.forwardModifier<Entity>(modifier);
		};

//-----------------------------------------------------------------------------

	auto crazyCannonShootingOrder =
		[](const World* w)
		{
			const float RANDOM_DEV = 100.f;
			return w->getMousePosition() + Vector2f(normalRandFloat(0.f,RANDOM_DEV),
			                                        normalRandFloat(0.f,RANDOM_DEV));
		};
	
	auto makeCannonCrazy =
		[](World& world, Time)
		{
			Modifier<Cannon> modifier;
			modifier.duration_ = Time();
			modifier.postFunction_ =
				[&](Cannon& c, Time)
				{
					c.setShootingOrderFunction(std::bind(crazyCannonShootingOrder, &world));
					c.modulateColor(sf::Color(200, 255, 0));
				};
			world.forwardModifier<Cannon>(modifier);
		};

	auto makeCannonSane =
		[](World& world, Time)
		{
			Modifier<Cannon> modifier;
			modifier.duration_ = Time();
			modifier.postFunction_ =
				[&](Cannon& c, Time)
				{
					c.setShootingOrderFunction(std::function<Vector2f()>(nullptr));
					c.modulateColor();
				};
			world.forwardModifier<Cannon>(modifier);
		};
	

//-----------------------------------------------------------------------------

	auto blackHoleInfluence =
		[](std::vector<Vector2f> positions, World& w, Time dt)
		{
			const float COEFF = 2.f;
			const float HORIZON_RADIUS = 40.f;
			std::vector<ecs::Entity> toRemove;
			
			ecs::EntityManager& ecs = w.getEntityManager();

			auto massicObjects = ecs.getObjectTable(ecs::Component::Massic | ecs::Component::Position);

			for(auto& massicPair : massicObjects)
			{
				Time deltaTime = dt;
			
				auto timeComp = dynCast<ecs::TimeArrow>
					(ecs.getComponent(massicPair.first, ecs::Component::TimeArrow));
				if(timeComp)
				{
					deltaTime *= timeComp->timeCoefficient_;
				}

				auto velComp = dynCast<ecs::Velocity>
					(massicPair.second[ecs::Component::Velocity]);
				assert(velComp);

				auto posComp = dynCast<ecs::Position>
					(massicPair.second[ecs::Component::Position]);
				assert(posComp);

				for(const auto& position : positions)
				{
					Vector2f blackHoleDir = position - posComp->position_;
					float distance = std::sqrt(std::pow(blackHoleDir.x, 2) + std::pow(blackHoleDir.y, 2));
					if(distance < HORIZON_RADIUS)
					{
						toRemove.push_back(massicPair.first);
					}
					blackHoleDir.normalize();
					blackHoleDir *= std::abs(w.getGravityVect().y) * COEFF;
					velComp->velocity_ += blackHoleDir * dt.asSeconds();
				}
			}

			for(const auto& ent : toRemove)
			{
				w.removeEntity(ent);
			}
		};

	auto blackHoleVisual =
		[](std::vector<Vector2f> positions, World& world, Time)
		{
			const float TRANSITION_DURATION = 0.25f;

			for(auto position : positions)
			{
				gui::Transition arrival (nullptr,
				                         gui::Transition::Quadratic,
				                         gui::TransformData(position,
				                                            0.f,
				                                            Vector2f(0.f, 0.f)),
				                         gui::TransformData(position,
				                                            0.f,
				                                            Vector2f(1.f, 1.f)),
				                         seconds(TRANSITION_DURATION));
				gui::Transition stayStill (nullptr,
				                           gui::Transition::Linear,
				                           gui::TransformData(position),
				                           gui::TransformData(position),
				                           seconds(DEFAULT_EVENT_DURATION * 1.5f - 2*TRANSITION_DURATION));
				gui::Transition extinction (nullptr,
				                            gui::Transition::Quadratic,
				                            gui::TransformData(position,
				                                               0.f,
				                                               Vector2f(1.f, 1.f)),
				                            gui::TransformData(position,
				                                               0.f,
				                                               Vector2f(0.f, 0.f)),
				                            seconds(TRANSITION_DURATION));
				gui::MultiTransition transitions;
				transitions.addTransition(arrival);
				transitions.addTransition(stayStill);
				transitions.addTransition(extinction);
				bool fadeOut = true, isInFront = true;
				world.addSprite(TextureID::BlackHole, "./media/sprites/BlackHole.png", sf::Color::White, transitions, !fadeOut, isInFront);
			}
		};
	
	auto makeBlackHole =
		[](int nBlackHole, World& world, Time)
		{
			const Vector2f middle (world.getWindowSize() / 2u);

			std::vector<Vector2f> positions;
			for(int i=0; i<nBlackHole; ++i)
			{
				float x;
				do {
					x = normalRandFloat(middle.x, middle.x / 2.f);
				} while(x < 0 || x > middle.x * 2);
		
				float y;
				do {
					y = normalRandFloat(middle.y, middle.y / 2.f);
				} while(y < 0 || y > middle.y * 2);

				Vector2f position (x, y);
				positions.push_back(position);
			}
			
			Modifier<World> worldMod;
			worldMod.duration_ = seconds(DEFAULT_EVENT_DURATION*1.5f);
			worldMod.preFunction_ = std::bind(blackHoleVisual, positions, std::placeholders::_1, std::placeholders::_2);
			worldMod.mainFunction_ = std::bind(blackHoleInfluence, positions, std::placeholders::_1, std::placeholders::_2);
			world.forwardModifier<World>(worldMod);

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
		reverseGravWorldMod.preFunction_ = std::bind(reverseGravWorld,
		                                             std::placeholders::_1,
		                                             std::placeholders::_2,
		                                             false);
		reverseGravWorldMod.postFunction_ = std::bind(reverseGravWorld,
		                                              std::placeholders::_1,
		                                              std::placeholders::_2,
		                                              true);
		                                              

		Modifier<World> generateObstaclesMod;
		generateObstaclesMod.duration_ = seconds(DEFAULT_EVENT_DURATION);
		generateObstaclesMod.preFunction_ = createObstacleModifiers;

		Modifier<World> addWindWorldMod;
		addWindWorldMod.duration_ = seconds(DEFAULT_EVENT_DURATION);
		addWindWorldMod.preFunction_ = addWindWorld;
		addWindWorldMod.postFunction_ = removeWindWorld;

		Modifier<World> makeCannonCrazyMod;
		makeCannonCrazyMod.duration_ = seconds(DEFAULT_EVENT_DURATION * 1.5f);
		makeCannonCrazyMod.preFunction_ = makeCannonCrazy;
		makeCannonCrazyMod.postFunction_ = makeCannonSane;

		Modifier<World> makeBlackHoleMod;
		makeBlackHoleMod.duration_ = seconds(DEFAULT_EVENT_DURATION * 1.5f);
		makeBlackHoleMod.preFunction_ = std::bind(makeBlackHole,
		                                          1,
		                                          std::placeholders::_1,
		                                          std::placeholders::_2);

		// Modifier<World> makeThreeBlackHolesMod;
		// makeThreeBlackHolesMod.duration_ = seconds(DEFAULT_EVENT_DURATION * 1.5f + 2.f);
		// makeThreeBlackHolesMod.preFunction_ = std::bind(makeBlackHole,
		//                                           3,
		//                                           std::placeholders::_1,
		//                                           std::placeholders::_2);

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

		Event makeCannonCrazyEvt;
		makeCannonCrazyEvt.diff = Event::Hard;
		makeCannonCrazyEvt.worldModifiers.push_back(makeCannonCrazyMod);

		Event makeBlackHoleEvt;
		makeBlackHoleEvt.diff = Event::Hard;
		makeBlackHoleEvt.worldModifiers.push_back(makeBlackHoleMod);		

		// Event makeThreeBlackHolesEvt;
		// makeThreeBlackHolesEvt.diff = Event::Debug;
		// makeThreeBlackHolesEvt.worldModifiers.push_back(makeThreeBlackHolesMod);


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
					addWindWorldEvt, gravAndTimeEvt, makeCannonCrazyEvt,
					makeBlackHoleEvt, makeThreeBlackHolesEvt};
		return events;
	}
}

