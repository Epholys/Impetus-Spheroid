#include "core/Cannon.hpp"
#include "core/World.hpp"
#include "core/Inventory.hpp"


//-----------------------------------------------------------------------------
// *** general datas: ***

namespace
{
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------
// *** constructor datas: ***

namespace
{
	const sf::Color CANNON_COLOR (80, 80, 80);
	const sf::Color CANNON_OUTLINE_COLOR (120, 120, 120);
	const float OUTLINE_THICKNESS = 5.f;

	const float CANNON_BODY_RADIUS = 35.f;
	const Vector2f CANNON_TUBE_SIZE { 60.f, 24.f };
}


//-----------------------------------------------------------------------------

Cannon::Cannon(const Vector2f& position, World& world, Inventory& inventory)
	: world_(world)
	, inventory_(inventory)
	, position_(position)
	, untilNextFire_(Time::Zero)
	, timeBeetweenFire_(milliseconds(333))
	, ballType_(Ball::Normal)
	, nTouchingBall_(1)
	, cannonBody_()
	, cannonTube_()
{
	for(int i=0; i<10; ++i)
	{
		ballBuffer_.push_back(std::make_pair(randomBallData(), Ball::Normal));
	}

	initView();
}

void Cannon::initView()
{
	cannonBody_.setRadius(CANNON_BODY_RADIUS);
	centerOrigin(cannonBody_);
	cannonBody_.setPosition(position_);
	cannonBody_.setFillColor(CANNON_COLOR);
	cannonBody_.setOutlineColor(CANNON_OUTLINE_COLOR);
	cannonBody_.setOutlineThickness(OUTLINE_THICKNESS);

	cannonTube_.setSize(CANNON_TUBE_SIZE);
	sf::FloatRect bounds = cannonTube_.getLocalBounds();
	cannonTube_.setOrigin(bounds.left, bounds.top + bounds.height / 2.f);
	cannonTube_.setPosition(position_);
	cannonTube_.setFillColor(CANNON_COLOR);
	cannonTube_.setOutlineColor(CANNON_OUTLINE_COLOR);
	cannonTube_.setOutlineThickness(OUTLINE_THICKNESS);
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

	updateTubeDirection();
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

void Cannon::updateTubeDirection()
{
	const float PI = 3.14159;
	auto mousePosition = world_.getMousePosition();
	auto gap = mousePosition - position_;
	float angle = std::atan(gap.y / gap.x) * 180 / PI;
	angle = (gap.x < 0) ? angle + 180 : angle;
	cannonTube_.setRotation(angle);
}


//-----------------------------------------------------------------------------

void Cannon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(cannonTube_, states);
	target.draw(cannonBody_, states);
	
	drawFutureBalls(target, states);
}

void Cannon::drawFutureBalls(sf::RenderTarget& target, sf::RenderStates states) const
{
	Vector2f ballPosition {2.5f, 570.f};
	Vector2f ballSpacing  {0.f, -60.f};
	for (const auto& data : ballBuffer_)
	{
		sf::CircleShape circ (Ball::RADIUS_);
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
	float cannonAngle =  cannonTube_.getRotation();
	if (!(cannonAngle < 10.f || cannonAngle > 265.f))
		return -1;

	const float IMPULSE_COEFF = 3.f;

	Entity::Ptr pBall (new Ball(world_,
	                            position_,
	                            world_.getGravityVect(),
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

	// Necessary not to have a weak cannon at low distances
	float impulse = std::max(std::max(direction.x, -direction.y),
	                         windowSize.x / 3.f);
	direction.normalize();			

	Vector2f velocity = IMPULSE_COEFF * impulse * direction / Ball::MASS_;
	velComp->velocity_ += velocity;

	ecs::Entity label = pBall->getLabel();

	// Pause Mass and Solid Component while the ball is inside the cannon
	float velocityNorm = std::sqrt(std::pow(velocity.x, 2) + std::pow(velocity.y, 2));
	sf::Time timeInsideCannon = seconds(CANNON_TUBE_SIZE.x / velocityNorm);
	world_.getEntityManager().pauseComponent(label, ecs::Component::Mass, timeInsideCannon);
	world_.getEntityManager().pauseComponent(label, ecs::Component::Solid, timeInsideCannon);

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
		ballBuffer_.front().first.color.a = 100;
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
