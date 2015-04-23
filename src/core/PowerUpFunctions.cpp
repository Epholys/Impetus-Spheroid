#include "utility/utility.hpp"
#include "ecs/ComponentCategory.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/ComponentTracker.hpp"
#include "core/PowerUpModifier.hpp"
#include "core/PowerUpToogle.hpp"
#include "core/PowerUpFunctions.hpp"
#include "core/Ball.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto switchGhostBalls =
		[](World& w)
	{
		w.switchBallType(Ball::Ghost);
	};

	auto switchNoGravBalls =
		[](World& w)
	{
		w.switchBallType(Ball::Massless);
	};

	auto cancelEvents =
		[](World& w)
	{
		w.cancelEvents(true);
	};

	auto switchAutoFire =
		[](World& w)
	{
		w.switchAutoFire();
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

		w.addModifier(removeTarget);
	};

	auto updateColor =
		[](Entity& ent, Time)
	{
		if(ent.getType() == EntityID::Target)
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
		const float MULTIPLIER = 1.5f;

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
		w.addEntityModifier(mod);
	};

	auto changeNTouching =
		[](World& w, int nTouching)
	{
		w.setNTouching(nTouching);
	};
}


//-----------------------------------------------------------------------------

void genPowerUps(std::map<PowerUpID::ID, std::shared_ptr<PowerUp>>& powerUps,
                 std::map<PowerUpID::ID, int>& numbers,
                 std::map<PowerUpID::ID, sf::Texture>& textures)
{
	using namespace PowerUpID;

// WARNING: WE DO NOT VERIFY IF THE TEXTURE IS SUCCESSFULLY LOADED

	PowerUpToogle* putGhost = new PowerUpToogle();
	putGhost->addActivateFunc(switchGhostBalls);
	putGhost->addDeactivateFunc(switchGhostBalls);
	std::shared_ptr<PowerUp> pPutGhost (putGhost);
	sf::Texture txtGhost;
	txtGhost.loadFromFile("./media/sprites/GhostBall.png");
	powerUps[GhostBall] = pPutGhost;
	numbers[GhostBall] = 0;
	textures[GhostBall] = txtGhost;

	PowerUpToogle* putNoGrav = new PowerUpToogle();
	putNoGrav->addActivateFunc(switchNoGravBalls);
	putNoGrav->addDeactivateFunc(switchNoGravBalls);
	std::shared_ptr<PowerUp> pPutNoGrav (putNoGrav);
	sf::Texture txtNoGrav;
	txtGhost.loadFromFile("./media/sprites/NoGravBall.png");
	powerUps[NoGravBall] = pPutNoGrav;
	numbers[NoGravBall] = 0;
	textures[NoGravBall] = txtGhost;

	PowerUpToogle* putCancel = new PowerUpToogle();
	putCancel->addActivateFunc(cancelEvents);
	std::shared_ptr<PowerUp> pPutCancel (putCancel);
	sf::Texture txtCancel;
	txtCancel.loadFromFile("./media/sprites/CancelEvents.png");
	powerUps[CancelEvents] = pPutCancel;
	numbers[CancelEvents] = 0;
	textures[CancelEvents] = txtCancel;
	
	PowerUpToogle* putAutoFire = new PowerUpToogle();
	putAutoFire->addActivateFunc(switchAutoFire);
	putAutoFire->addDeactivateFunc(switchAutoFire);
	std::shared_ptr<PowerUp> pPutAutoFire (putAutoFire);
	sf::Texture txtAutoFire;
	txtAutoFire.loadFromFile("./media/sprites/AutoFire.png");
	powerUps[AutoFire] = pPutAutoFire;
	numbers[AutoFire] = 0;
	textures[AutoFire] = txtAutoFire;
	
	Modifier<World> modTime;
	modTime.postFunction_ = addTime;
	modTime.duration_ = Time();
	PowerUpModifier* pumTime = new PowerUpModifier();
	pumTime->addModifier(modTime);
	std::shared_ptr<PowerUp> pPumTime (pumTime);
	sf::Texture txtTime;
	txtTime.loadFromFile("./media/sprites/AddTime.png");
	powerUps[AddTime] = pPumTime;
	numbers[AddTime] = 0;
	textures[AddTime] = txtTime;

	Modifier<World> modTarget;
	modTarget.postFunction_ = addTarget;
	modTarget.duration_ = Time();
	PowerUpModifier* pumTarget = new PowerUpModifier();
	pumTarget->addModifier(modTarget);
	std::shared_ptr<PowerUp> pPumTarget (pumTarget);
	sf::Texture txtTarget;
	txtTarget.loadFromFile("./media/sprites/AddTarget.png");
	powerUps[AddTarget] = pPumTarget;
	numbers[AddTarget] = 0;
	textures[AddTarget] = txtTarget;

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
	powerUps[PointMultiplier] = pPumMult;
	numbers[PointMultiplier] = 0;
	textures[PointMultiplier] = txtMult;

	PowerUpToogle* putTouching = new PowerUpToogle();
	putTouching->addActivateFunc(std::bind(changeNTouching,
	                                       std::placeholders::_1,
	                                       2));
	putTouching->addDeactivateFunc(std::bind(changeNTouching,
	                                         std::placeholders::_1,
	                                         1));
	std::shared_ptr<PowerUp> pPutTouching (putTouching);
	sf::Texture txtTouching;
	txtTouching.loadFromFile("./media/sprites/BallTouchDouble.png");
	powerUps[BallTouchDouble] = pPutTouching;
	numbers[BallTouchDouble] = 0;
	textures[BallTouchDouble] = txtTouching;
}
