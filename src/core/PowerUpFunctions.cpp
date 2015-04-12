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
	
}
