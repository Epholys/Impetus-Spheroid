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

	const Vector2f BALL_POSITION {2.5f, 570.f};
	const Vector2f BALL_SPACING  {0.f, -40.f};
	const Time TRANSITION_DURATION = milliseconds(125);
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
	, ballBuffer_()
	, transitionDeque_(gui::TransformData(BALL_POSITION),
	                   gui::TransformData(BALL_SPACING, 0.f, Vector2f()),
	                   gui::Transition::Linear,
	                   TRANSITION_DURATION)
	, cannonBody_()
	, cannonTube_()
	, arcPreview_(sf::TrianglesStrip)
{
	for(int i=0; i<15; ++i)
	{
		BufferEntry entry { randomBallData(), Ball::Normal, sf::CircleShape(Ball::RADIUS_) };
		entry.shape.setFillColor(entry.data.color);
		ballBuffer_.push_back(entry);
		transitionDeque_.pushBack(&(ballBuffer_.back().shape));
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

	transitionDeque_.update(dt);
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

void Cannon::updateArcPreview()
{
	computeArcView(computeBallTrajectory());
}

std::vector<Vector2f> Cannon::computeBallTrajectory()
{
	const Time FLIGHT_DURATION = seconds(5.f);
	const Time FLIGHT_DT = seconds(1 / 60.f);
	int pointCount = std::ceil(FLIGHT_DURATION.asSeconds() / FLIGHT_DT.asSeconds());
	Vector2f ballVelocity = computeBallVelocity(world_.getMousePosition());
	float velocityNorm = std::sqrt(std::pow(ballVelocity.x, 2.f) + std::pow(ballVelocity.y, 2.f));
	sf::Time timeInsideCannon = seconds(CANNON_TUBE_SIZE.x / velocityNorm);
	auto gravityVect = world_.getGravityVect();
	std::vector<Vector2f> arcPoint;
	arcPoint.push_back(position_);
	Time totalFlight;
	for(int i=1; i<pointCount; ++i)
	{
		if(totalFlight >= timeInsideCannon)
		{
			ballVelocity += gravityVect * FLIGHT_DT.asSeconds();
		}
		arcPoint.push_back(arcPoint.at(i-1) + ballVelocity * FLIGHT_DT.asSeconds());
		totalFlight += FLIGHT_DT;
	}
	return arcPoint;
}
void Cannon::computeArcView(const std::vector<Vector2f>& trajectory)
{
	const int PREVIEW_SIZE_BEGIN = 2;
	float previewSize = PREVIEW_SIZE_BEGIN;
	arcPreview_.clear();
	auto firstpos = trajectory.at(0);
	arcPreview_.append(sf::Vertex(Vector2f(firstpos.x - previewSize, firstpos.y), sf::Color(0,0,0,50)));
	arcPreview_.append(sf::Vertex(Vector2f(firstpos.x + previewSize, firstpos.y), sf::Color(0,0,0,50)));
	for(std::size_t i=1; i<trajectory.size(); ++i)
	{
		auto pos = trajectory.at(i);
		Vector2f dir = pos - trajectory.at(i-1);
		dir.normalize();
		Vector2f orth (-dir.y, dir.x);
		sf::Vertex vx1 (Vector2f(pos.x - orth.x * previewSize,
		                         pos.y - orth.y * previewSize),
		                sf::Color(0,0,0,50));
		sf::Vertex vx2 (Vector2f(pos.x + orth.x * previewSize,
		                         pos.y + orth.y * previewSize),
		                sf::Color(0,0,0,50));
		arcPreview_.append(vx1);
		arcPreview_.append(vx2);
	}
}


//-----------------------------------------------------------------------------

void Cannon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(!world_.hasStarted())
	{
		target.draw(arcPreview_, states);
	}
	target.draw(cannonTube_, states);
	target.draw(cannonBody_, states);
	
	for (const auto& entry : ballBuffer_)
	{
		target.draw(entry.shape, states);
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

	Entity::Ptr pBall (new Ball(world_,
	                            position_,
	                            world_.getGravityVect(),
	                            ballBuffer_.front().data,
	                            nTouchingBall_,
	                            ballBuffer_.front().type));

	auto velComp = dynCast<ecs::Velocity>
		(world_.getEntityManager().getComponent(pBall->getLabel(),
		                                        ecs::Component::Velocity));
	assert(velComp);

	Vector2f velocity = computeBallVelocity(world_.getMousePosition());
	velComp->velocity_ += velocity;

	ecs::Entity label = pBall->getLabel();

	// Pause Mass and Solid Component while the ball is inside the cannon
	float velocityNorm = std::sqrt(std::pow(velocity.x, 2.f) + std::pow(velocity.y, 2.f));
	sf::Time timeInsideCannon = seconds(CANNON_TUBE_SIZE.x / velocityNorm);
	world_.getEntityManager().pauseComponent(label, ecs::Component::Mass, timeInsideCannon);
	world_.getEntityManager().pauseComponent(label, ecs::Component::Solid, timeInsideCannon);

	world_.addEntity(std::move(pBall));

	updateBuffer();
	
	applyBallType();
	updateInventory();

	return label;
}

Vector2f Cannon::computeBallVelocity(Vector2f mousePosition)
{
	const float IMPULSE_COEFF = 3.f;
	
	// Necessary not to have ball at the speed of light in peculiar kind of
	// zooming
	Vector2f windowSize = Vector2f(world_.getWindowSize());
	mousePosition = { std::min(mousePosition.x, windowSize.x)
	                  , std::min(mousePosition.y, windowSize.y) };


	Vector2f direction = mousePosition - position_;

	// Necessary not to have a (too) weak cannon at low distances
	float impulse = std::max(std::max(std::abs(direction.x), std::abs(direction.y)),
	                         windowSize.x / 5.f);

	direction.normalize();			

	Vector2f velocity = IMPULSE_COEFF * impulse * direction / Ball::MASS_;
   
	return velocity;
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
	ballBuffer_.front().type = ballType_;

	if (ballType_ == Ball::Normal)
	{
		ballBuffer_.front().data.color.a = 255;
	}

	else if(ballType_ & Ball::Ghost)
	{
		ballBuffer_.front().data.color.a = 100;
	}
}

void Cannon::updateBuffer()
{
	transitionDeque_.popFront();
	ballBuffer_.pop_front();
	BufferEntry entry { randomBallData(), Ball::Normal, sf::CircleShape(Ball::RADIUS_) };
	entry.shape.setFillColor(entry.data.color);
	ballBuffer_.push_back(entry);
	transitionDeque_.pushBack(&(ballBuffer_.back().shape));
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







