#include "utility/utility.hpp"
#include "ecs/ComponentCategory.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/ComponentTracker.hpp"
#include "core/PowerUpModifier.hpp"
#include "core/PowerUpToogle.hpp"
#include "core/PowerUpFunctions.hpp"
#include "core/Ball.hpp"
#include "core/Inventory.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto switchGhostBalls =
		[](World& w)
	{
		Modifier<Cannon> ghostBalls;
		ghostBalls.duration_ = Time();
		ghostBalls.postFunction_ =
			[](Cannon& c, Time)
			{
				c.switchBallType(Ball::Ghost);
			};
		w.forwardModifier<Cannon>(ghostBalls);
	};

	auto switchNoGravBalls =
		[](World& w)
	{
		Modifier<Cannon> masslessBalls;
		masslessBalls.duration_ = Time();
		masslessBalls.postFunction_ =
			[](Cannon& c, Time)
			{
				c.switchBallType(Ball::Massless);
			};
		w.forwardModifier<Cannon>(masslessBalls);
	};

	auto cancelEvents =
		[](World& w)
	{
		w.cancelEvents(true);
		w.addSprite(TextureID::CancelEventFlash,
		            "./media/sprites/CancelEventFlash.png",
		            gui::Transition(nullptr,
		                            gui::Transition::Linear,
		                            gui::TransformData(Vector2f(w.getWindowSize()) / 2.f),
		                            gui::TransformData(Vector2f(w.getWindowSize()) / 2.f,
		                                               360.f,
		                                               Vector2f(0.25f, 0.25f)),
		                            seconds(.5f)),
		            true);
	};
	
	auto addTime = 
		[](World& w, Time)
	{
		Time TIME_TO_ADD = seconds(5);

		w.addTime(TIME_TO_ADD);
	};

	auto addTarget =
		[](World& w, Time)
	{
		Time DURATION_OF_TARGET = seconds(20);
		
		auto windowSize = w.getWindowSize();
		auto label = w.createTarget(Vector2f(windowSize.x * 0.75f, windowSize.y * 0.5f));
		
		Modifier<World> removeTarget;
		removeTarget.preDelay_ = DURATION_OF_TARGET;
		removeTarget.postFunction_ =
			[label](World& w, Time)
			{
				w.removeEntity(label);
			};

		w.forwardModifier(removeTarget);
	};

	auto updateColor =
		[](Entity& ent, Time)
	{
		if(ent.getType() == EntityType::Target)
		{
			Entity* pEnt = &ent;
			Target* pTarg = dynamic_cast<Target*>(pEnt);
			assert(pTarg);
			pTarg->updateColor();
		}
	};


	auto multiplyPoints =
		[](World& w, Time, bool undo)
	{
		const float MULTIPLIER = PowerUpDatas::TARGET_POINT_MULTIPLIER;

		auto& em = w.getEntityManager();
		auto targetComps = em.getAllComponents(ecs::Component::Target);
		for (auto it=targetComps.begin(); it!=targetComps.end(); ++it)
		{
			auto targetComponent = dynCast<ecs::Target>(*it);
			if(targetComponent)
			{
				float mult = 0.f;
				if(!undo)
				{
				    mult = targetComponent->getPointMultiplier() * MULTIPLIER;
				}
				else
				{
					mult = targetComponent->getPointMultiplier() / MULTIPLIER;
				}
				targetComponent->setPointMultiplier(mult);
			}
		}

		Modifier<Entity> mod;
		mod.postFunction_ = updateColor;
		mod.duration_ = Time();
		w.forwardModifier<Entity>(mod);
	};

	auto changeNTouching =
		[](World& w, int nTouching)
	{
		Modifier<Cannon> multipleTouch;
		multipleTouch.duration_ = Time();
		multipleTouch.postFunction_ =
			[nTouching](Cannon& c, Time)
			{
				c.setNTouching(nTouching);
			};
		w.forwardModifier<Cannon>(multipleTouch);
	};
}


//-----------------------------------------------------------------------------

void genPowerUps(std::map<PowerUpID::ID, PowerUpEntry>& powerUpTable)
{
	using namespace PowerUpID;

// WARNING: WE DO NOT VERIFY IF THE TEXTURES ARE SUCCESSFULLY LOADED
	
	PowerUpToogle* putGhost = new PowerUpToogle();
	putGhost->addActivateFunc(switchGhostBalls);
	putGhost->addDeactivateFunc(switchGhostBalls);
	std::shared_ptr<PowerUp> pPutGhost (putGhost);
	sf::Texture txtGhost;
	txtGhost.loadFromFile("./media/sprites/GhostBall.png");
	auto* entry = &powerUpTable[GhostBall];
	entry->powerUp = pPutGhost;
	entry->stock = 0;
	entry->texture = txtGhost;

	PowerUpToogle* putNoGrav = new PowerUpToogle();
	putNoGrav->addActivateFunc(switchNoGravBalls);
	putNoGrav->addDeactivateFunc(switchNoGravBalls);
	std::shared_ptr<PowerUp> pPutNoGrav (putNoGrav);
	sf::Texture txtNoGrav;
	txtGhost.loadFromFile("./media/sprites/NoGravBall.png");
	entry = &powerUpTable[NoGravBall];
	entry->powerUp = pPutNoGrav;
	entry->stock = 0;
	entry->texture = txtGhost;

	PowerUpToogle* putCancel = new PowerUpToogle();
	putCancel->addActivateFunc(cancelEvents);
	std::shared_ptr<PowerUp> pPutCancel (putCancel);
	sf::Texture txtCancel;
	txtCancel.loadFromFile("./media/sprites/CancelEvents.png");
	entry = &powerUpTable[CancelEvents];
	entry->powerUp = pPutCancel;
	entry->stock = 0;
	entry->texture = txtCancel;
		
	Modifier<World> modTime;
	modTime.postFunction_ = addTime;
	modTime.duration_ = Time();
	PowerUpModifier* pumTime = new PowerUpModifier();
	pumTime->addModifier(modTime);
	std::shared_ptr<PowerUp> pPumTime (pumTime);
	sf::Texture txtTime;
	txtTime.loadFromFile("./media/sprites/AddTime.png");
	entry = &powerUpTable[AddTime];
	entry->powerUp = pPumTime;
	entry->stock = 0;
	entry->texture = txtTime;

	Modifier<World> modTarget;
	modTarget.postFunction_ = addTarget;
	modTarget.duration_ = Time();
	PowerUpModifier* pumTarget = new PowerUpModifier();
	pumTarget->addModifier(modTarget);
	std::shared_ptr<PowerUp> pPumTarget (pumTarget);
	sf::Texture txtTarget;
	txtTarget.loadFromFile("./media/sprites/AddTarget.png");
	entry = &powerUpTable[AddTarget];
	entry->powerUp = pPumTarget;
	entry->stock = 0;
	entry->texture = txtTarget;

	const Time MULTIPLIER_TIME = seconds(10);
	Modifier<World> modMult;
	modMult.duration_ = MULTIPLIER_TIME;
	modMult.preFunction_ = std::bind(multiplyPoints, std::placeholders::_1, std::placeholders::_2, false);
	modMult.postFunction_ = std::bind(multiplyPoints, std::placeholders::_1, std::placeholders::_2, true);
	PowerUpModifier* pumMult = new PowerUpModifier();
	pumMult->addModifier(modMult);
	std::shared_ptr<PowerUp> pPumMult (pumMult);
	sf::Texture txtMult;
	txtMult.loadFromFile("./media/sprites/MultiplyPoints.png");
	entry = &powerUpTable[PointMultiplier];
	entry->powerUp = pPumMult;
	entry->stock = 0;
	entry->texture = txtMult;

	const int N_BALL_TOUCH = 2;
	PowerUpToogle* putTouching = new PowerUpToogle();
	putTouching->addActivateFunc(std::bind(changeNTouching,
	                                       std::placeholders::_1,
	                                       N_BALL_TOUCH));
	putTouching->addDeactivateFunc(std::bind(changeNTouching,
	                                         std::placeholders::_1,
	                                         1));
	std::shared_ptr<PowerUp> pPutTouching (putTouching);
	sf::Texture txtTouching;
	txtTouching.loadFromFile("./media/sprites/BallTouchDouble.png");
	entry = &powerUpTable[BallTouchDouble];
	entry->powerUp = pPutTouching;
	entry->stock = 0;
	entry->texture = txtTouching;
}
