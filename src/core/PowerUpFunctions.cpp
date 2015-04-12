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
	numbers[GhostBall] = 10;
	textures[GhostBall] = txtGhost;

	PowerUpToogle* putNoGrav = new PowerUpToogle();
	putNoGrav->addActivateFunc(switchNoGravBalls);
	putNoGrav->addDeactivateFunc(switchNoGravBalls);
	std::shared_ptr<PowerUp> pPutNoGrav (putNoGrav);
	sf::Texture txtNoGrav;
	txtGhost.loadFromFile("./media/sprites/NoGravBall.png");
	powerUps[NoGravBall] = pPutNoGrav;
	numbers[NoGravBall] = 10;
	textures[NoGravBall] = txtGhost;

	PowerUpToogle* putCancel = new PowerUpToogle();
	putCancel->addActivateFunc(cancelEvents);
	std::shared_ptr<PowerUp> pPutCancel (putCancel);
	sf::Texture txtCancel;
	txtCancel.loadFromFile("./media/sprites/CancelEvents.png");
	powerUps[CancelEvents] = pPutCancel;
	numbers[CancelEvents] = 10;
	textures[CancelEvents] = txtCancel;
	
	PowerUpToogle* putAutoFire = new PowerUpToogle();
	putAutoFire->addActivateFunc(switchAutoFire);
	putAutoFire->addDeactivateFunc(switchAutoFire);
	std::shared_ptr<PowerUp> pPutAutoFire (putAutoFire);
	sf::Texture txtAutoFire;
	txtAutoFire.loadFromFile("./media/sprites/AutoFire.png");
	powerUps[AutoFire] = pPutAutoFire;
	numbers[AutoFire] = 100;
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
	numbers[AddTime] = 3;
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
	numbers[AddTarget] = 3;
	textures[AddTarget] = txtTarget;
}
