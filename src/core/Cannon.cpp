#include "core/Cannon.hpp"
#include "core/World.hpp"
#include "core/Inventory.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------

Cannon::Cannon(const Vector2f& position, World& world, Inventory& inventory)
	: world_(world)
	, inventory_(inventory)
	, position_(position)
	, untilNextFire_(Time::Zero)
	, timeBeetweenFire_(milliseconds(333))
	, ballMass_(1.f)
	, ballRadius_(10.f)
	, ballType_(Ball::Normal)
	, nTouchingBall_(1)
{
	for(int i=0; i<10; ++i)
	{
		ballBuffer_.push_back(std::make_pair(randomBallData(), Ball::Normal));
	}
}

Cannon::~Cannon()
{
}


//-----------------------------------------------------------------------------

void Cannon::update(Time dt)
{
	applyModifiers(*this, dt);
	cleanModifiers();

	applyAutoFire(dt);
}

void Cannon::applyAutoFire(Time dt)
{
	untilNextFire_ -= dt;

	if(untilNextFire_ <= Time() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		createBall();
		untilNextFire_ = timeBeetweenFire_;
	}

}


//-----------------------------------------------------------------------------

void Cannon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	drawFutureBalls(target, states);
}

void Cannon::drawFutureBalls(sf::RenderTarget& target, sf::RenderStates states) const
{
	Vector2f ballPosition {2.5f, 570.f};
	Vector2f ballSpacing  {0.f, -60.f};
	for (const auto& data : ballBuffer_)
	{
		sf::CircleShape circ (ballRadius_);
		circ.setFillColor(data.first.color);
		circ.setPosition(ballPosition);
		ballPosition += ballSpacing;
		target.draw(circ, states);
	}
}


//-----------------------------------------------------------------------------

void Cannon::handleInput(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed &&
	         event.mouseButton.button == sf::Mouse::Left)
	{
		createBall();
		untilNextFire_ = timeBeetweenFire_;
	}
}


//-----------------------------------------------------------------------------

void Cannon::switchBallType(unsigned int type)
{
	ballType_ ^= type;
	applyBallType();
}

void Cannon::setNTouching(int nTouching)
{
	nTouchingBall_ = nTouching;
}

ecs::Entity Cannon::createBall()
{
	const float IMPULSE_COEFF = 3.f;

	Entity::Ptr pBall (new Ball(&world_, world_.getEntityManager(),
	                            position_,
	                            ballRadius_, ballMass_, world_.getGravityVect(),
	                            ballBuffer_.front().first,
	                            nTouchingBall_,
	                            ballBuffer_.front().second));

	auto velComp = dynCast<ecs::Velocity>
		(world_.getEntityManager().getComponent(pBall->getLabel(),
		                                         ecs::Component::Velocity));
	assert(velComp);

	// Necessary not to have ball at the speed of light in peculiar kind of
	// zooming
	Vector2f mousePosition = world_.getMousePosition();
	Vector2f windowSize = Vector2f(world_.getWindowSize());
	mousePosition = { std::min(mousePosition.x, windowSize.x)
					, std::min(mousePosition.y, windowSize.y) };


	Vector2f direction = mousePosition - position_;
	float impulse = std::max(direction.x, -direction.y);
	direction.normalize();			

	velComp->velocity_ += IMPULSE_COEFF * impulse * direction / ballMass_;

	ecs::Entity label = pBall->getLabel();

	world_.addEntity(std::move(pBall));

	ballBuffer_.pop_front();

	ballBuffer_.push_back(std::make_pair(randomBallData(), Ball::Normal));
	
	applyBallType();
	updateInventory();

	return label;
}

void Cannon::updateInventory()
{
	if(ballType_ & Ball::Ghost)
		inventory_.decrement(PowerUpID::GhostBall);
	if(ballType_ & Ball::Massless)
		inventory_.decrement(PowerUpID::NoGravBall);
	if(nTouchingBall_ != 1)
		inventory_.decrement(PowerUpID::BallTouchDouble);
}

void Cannon::applyBallType()
{
	ballBuffer_.front().second = ballType_;

	if (ballType_ == Ball::Normal)
	{
		ballBuffer_.front().first.color.a = 255;
	}

	else if(ballType_ & Ball::Ghost)
	{
		ballBuffer_.front().first.color.a = 155;
	}
}

BallData Cannon::randomBallData() const
{
	for(auto ritr = ballDatas.rbegin(); ritr != ballDatas.rend(); ++ritr)
	{
		int rint = randInt(1,1000);
		if(rint <= (*ritr).proba)
		{
			return *ritr;
			break;
		}
	}
	return ballDatas[0];
}
