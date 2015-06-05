#include "core/Cannon.hpp"

//-----------------------------------------------------------------------------

Cannon::Cannon(const Vector2f& windowSize)
{
}

Cannon::~Cannon()
{
}


//-----------------------------------------------------------------------------

void Cannon::handleInput(const sf::Event& event)
{
}

void Cannon::update(Time dt)
{
}

void Cannon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
}


//-----------------------------------------------------------------------------

ecs::Entity Cannon::createBall(Vector2f mousePosition)
{
	// const Vector2f CANNON_POSITION {40.f, 580.f};
	// const float IMPULSE_COEFF = 3.f;

	// Entity::Ptr pBall (new Ball(this, ecs_,
	//                             CANNON_POSITION,
	//                             ballRadius_, ballMass_, gravityVect_,
	//                             ballBuffer_.front().first,
	//                             nTouchingBall_,
	//                             ballBuffer_.front().second));

	// auto velComp = dynCast<ecs::Velocity>
	// 	(ecs_.getComponent(pBall->getLabel(), ecs::Component::Velocity));
	// assert(velComp);

	// Vector2f direction = mousePosition - CANNON_POSITION;
	// float impulse = std::max(direction.x, -direction.y);
	// direction.normalize();
			

	// velComp->velocity_ += IMPULSE_COEFF * impulse * direction / ballMass_;

	// ecs::Entity label = pBall->getLabel();

	// entities_.push_back(std::move(pBall));


	// //Quick and dirty I
	// if(ballType_ & Ball::Ghost)
	// 	inventory_.decrement(PowerUpID::GhostBall);
	// if(ballType_ & Ball::Massless)
	// 	inventory_.decrement(PowerUpID::NoGravBall);
	// if(nTouchingBall_ != 1)
	// 	inventory_.decrement(PowerUpID::BallTouchDouble);

	// ballBuffer_.pop_front();


	// ballBuffer_.push_back(std::make_pair(genBallData(), Ball::Normal));
	
	// applyBallType();

	// return label;
}
