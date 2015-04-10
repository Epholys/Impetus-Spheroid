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
}


//-----------------------------------------------------------------------------

void genPowerUps(std::map<PowerUpID::ID, std::shared_ptr<PowerUp>>& powerUps,
                 std::map<PowerUpID::ID, int>& numbers,
                 std::map<PowerUpID::ID, sf::Texture>& textures)
{
	using namespace PowerUpID;

	PowerUpToogle* putGhost = new PowerUpToogle();
	putGhost->addActivateFunc(switchGhostBalls);
	putGhost->addDeactivateFunc(switchGhostBalls);
	std::shared_ptr<PowerUp> pPutGhost (putGhost);
	sf::Texture txtGhost;
// DO NOT VERIFY IF THE TEXTURE IS SUCCESSFULLY LOADED
	txtGhost.loadFromFile("./media/sprites/GhostBall.png");
	powerUps[GhostBall] = pPutGhost;
	numbers[GhostBall] = 10;
	textures[GhostBall] = txtGhost;
}
