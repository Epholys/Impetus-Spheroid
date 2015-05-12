#include "core/World.hpp"
#include "core/TransGamesData.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------
// *** constructor: ***

const Time World::TIME_BEETWEEN_FIRE = milliseconds(333);

World::World(sf::RenderWindow& window, TransGamesData& datas, int precision)
	: window_(window)
	, ecs_()
	, physEng_(ecs_, precision)
	, evtGen_()
	, difficulty_(DifficultyContext{this, &evtGen_, &datas, &(datas.inventory)})
	, inventory_(datas.inventory)
	, cannon_(Vector2f(window.getSize()))
	, state_(Waiting)
	, autoFireOn_(false)
	, untilNextFire_(Time::Zero)
	, speedCoeff_(1.f)
	, entities_()
	, entitiesModifiers_()
	, ballType_(Ball::Normal)
	, nTouchingBall_(1)
	, gravityVect_(0.f, 1000.f)
	, ballBuffer_()
	, ballMass_(1.f)
	, ballRadius_(10.f)
{
	const Vector2f SCORE_POSITION (750.f, 0.f);

	generateWorld();
}

void World::generateWorld()
{
	auto winSize = window_.getSize();

	Entity::Ptr ceiling (new Wall(this, ecs_,
	                              Vector2f(winSize.x / 2.f, 0.f),
	                              Vector2f(winSize.x, 10.f),
	                              sf::Color(80,80,80)));
	
	Entity::Ptr leftWall (new Wall(this, ecs_,
	                               Vector2f(12.5f, winSize.y / 2.f),
	                               Vector2f(25.f, winSize.y),
	                               sf::Color(80,80,80)));

	Entity::Ptr rightWall (new Wall(this, ecs_,
	                                Vector2f(winSize.x, winSize.y / 2.f),
	                                Vector2f(10.f, winSize.y),
	                                sf::Color(80,80,80)));

	entities_.push_back(std::move(ceiling));
	entities_.push_back(std::move(leftWall));
	entities_.push_back(std::move(rightWall));

	createTarget(Vector2f(3* window_.getSize().x / 4.f, window_.getSize().y / 2.f));

	for(int i=0; i<10; ++i)
	{
	ballBuffer_.push_back(std::make_pair(genBallData(), Ball::Normal));
	}
}


ecs::EntityManager& World::getEntityManager()
{
	return ecs_;
}

Vector2f& World::getGravityVect()
{
	return gravityVect_;
}

Vector2u World::getWindowSize() const
{
	return Vector2u(window_.getSize());
}

void World::addEntityModifier(Modifier<Entity> modifier)
{
	entitiesModifiers_.push_back(modifier);
}

void World::addEntity(Entity::Ptr entity)
{
	entities_.push_back(std::move(entity));
}

void World::removeEntity(ecs::Entity label)
{
	ecs_.removeEntity(label);
	
	entities_.erase(
		std::remove_if(entities_.begin(),
		               entities_.end(),
		               [label](const Entity::Ptr& pe)
		               {
			               return pe->getLabel() == label;
		               }),
		entities_.end());
}

bool World::isGameOver() const
{
	return state_ == GameOver;
}

void World::setState(GameState state)
{
	state_ = state;
}

void World::switchBallType(unsigned int type)
{
	ballType_ ^= type;
	applyBallType();
}

void World::cancelEvents(bool comeFromInventory)
{
	Modifiable<World>::forceEndingModifiers(*this);
	if(comeFromInventory)
	{
		inventory_.decrement(PowerUpID::CancelEvents);
	}
}

void World::addTime(Time adding)
{
	difficulty_.addTime(adding);
}

void World::setNTouching(int n)
{
	nTouchingBall_ = n;
}

void World::updateDifficulty(DifficultyWorld diff)
{
	speedCoeff_ += diff.speedConstant;
}

const std::vector<eg::PhysicEngine::entityPair>&
World::getTrackedCollisions() const
{
	return physEng_.getTrackedCollisions();
}

//-----------------------------------------------------------------------------
// *** gameloop functions: ***
ecs::Entity World::createBall(Vector2f mousePosition)
{
	const Vector2f CANON_POSITION {40.f, 580.f};
	const float IMPULSE_COEFF = 3.f;

	Entity::Ptr pBall (new Ball(this, ecs_,
	                            CANON_POSITION,
	                            ballRadius_, ballMass_, gravityVect_,
	                            ballBuffer_.front().first,
	                            nTouchingBall_,
	                            ballBuffer_.front().second));

	auto velComp = dynCast<ecs::Velocity>
		(ecs_.getComponent(pBall->getLabel(), ecs::Component::Velocity));
	assert(velComp);

	Vector2f direction = mousePosition - CANON_POSITION;
	float impulse = std::max(direction.x, -direction.y);
	direction.normalize();
			

	velComp->velocity_ += IMPULSE_COEFF * impulse * direction / ballMass_;

	ecs::Entity label = pBall->getLabel();

	entities_.push_back(std::move(pBall));


	//Quick and dirty I
	if(ballType_ & Ball::Ghost)
		inventory_.decrement(PowerUpID::GhostBall);
	if(ballType_ & Ball::Massless)
		inventory_.decrement(PowerUpID::NoGravBall);
	if(nTouchingBall_ != 1)
		inventory_.decrement(PowerUpID::BallTouchDouble);

	ballBuffer_.pop_front();


	ballBuffer_.push_back(std::make_pair(genBallData(), Ball::Normal));
	
	applyBallType();

	return label;
}

// Quick and dirty II
void World::applyBallType()
{
	ballBuffer_.front().first.color.a = 255;
	ballBuffer_.front().second = Ball::Normal;

	if(ballType_ & Ball::Ghost)
	{
		ballBuffer_.front().first.color.a = 155;
		ballBuffer_.front().second = ballType_;
	}
	else if(ballType_ & Ball::Massless)
	{
		ballBuffer_.front().second = ballType_;
	}
}

BallData World::genBallData() const
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

ecs::Entity World::createTarget(Vector2f position)
{
	const Vector2f TARGET_DIMENSION {15.f, 75.f};
	const sf::Color TARGET_COLOR {sf::Color::Yellow};


	Entity::Ptr pTarget (new Target(this, ecs_,
	                                position,
	                                TARGET_DIMENSION,
	                                TARGET_COLOR));
	ecs::Entity label = pTarget->getLabel();

	entities_.push_back(std::move(pTarget));

	return label;
}

void World::handleInput(const sf::Event& event)
{
	if(state_ == Waiting || state_ == GameOver)
	{
		if(event.type == sf::Event::MouseButtonPressed)
			state_ = Playing;
		else
			return;
	}

	else if (event.type == sf::Event::MouseButtonPressed &&
	         event.mouseButton.button == sf::Mouse::Left)
	{
		createBall(Vector2f(sf::Mouse::getPosition(window_)));
		untilNextFire_ = TIME_BEETWEEN_FIRE;
	}

	difficulty_.handleInput(event);
}

void World::update(Time dt)
{
	if(state_ == Waiting)
		return;

	// All the objects that requires the "real" time:
	if(state_ != GameOver)
	{
		difficulty_.update(dt);
	}

	getEvent(dt);

	// All the objects that requires the faster time below:
	dt *= speedCoeff_;

	applyAutoFire(dt);

	ecs_.update(dt);
	applyModifiers(dt);

	for(const auto& ent : entities_)
	{
		ent->update(dt);
	}
	physEng_.update(dt);

	cleanEntities();
	cleanModifiers();
}

void World::getEvent(Time dt)
{
	evt::Event evt = evtGen_.update(dt);

	if(evt.diff == evt::Event::None)
		return;
	
	if(!evt.entityModifiers.empty())
	{
		entitiesModifiers_.insert(entitiesModifiers_.begin(),
		                          evt.entityModifiers.begin(),
		                          evt.entityModifiers.end());
	}
	if(!evt.worldModifiers.empty())
	{
		modifiers_.insert(modifiers_.begin(),
		                  evt.worldModifiers.begin(),
		                  evt.worldModifiers.end());
	}
}

void World::applyAutoFire(Time dt)
{
	untilNextFire_ -= dt;

	if(state_ != Waiting && state_ != GameOver &&
	   untilNextFire_ <= Time() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		createBall(Vector2f(sf::Mouse::getPosition(window_)));
		untilNextFire_ = TIME_BEETWEEN_FIRE;
	}
}

void World::cleanModifiers()
{
	Modifiable<World>::cleanModifiers();

	entitiesModifiers_.clear();
}

void World::applyModifiers(Time dt)
{
	Modifiable<World>::applyModifiers(*this, dt);

	for(auto& entity : entities_)
	{
		for(auto& modifier : entitiesModifiers_)
		{
			entity->addModifier(modifier);
		}
	}
}

void World::cleanEntities()
{
	auto it = entities_.begin();

	while(it != entities_.end())
	{
		auto posPtr = ecs_.getComponent((*it)->getLabel(),
		                                ecs::Component::Position);
		auto collPtr = ecs_.getComponent((*it)->getLabel(),
		                                 ecs::Component::Collidable);
		if(posPtr && collPtr)
		{
			sf::Vector2f windowSize (window_.getSize());

			auto posComp = dynCast<ecs::Position>(posPtr);
			assert(posComp);

			auto sphereCollComp = dynCast<ecs::CollidableSphere>(collPtr);
			auto rectCollComp = dynCast<ecs::CollidableRect>(collPtr);

			if((sphereCollComp &&
			    (posComp->position_.y - sphereCollComp->radius_ > windowSize.y ||
			     posComp->position_.y + sphereCollComp->radius_ < 0 ||
			     posComp->position_.x - sphereCollComp->radius_ > windowSize.x ||
			     posComp->position_.x + sphereCollComp->radius_ < 0)) ||
			   (rectCollComp &&
			    (posComp->position_.y - rectCollComp->size_.y/2.f > windowSize.y ||
			     posComp->position_.y + rectCollComp->size_.y/2.f < 0 ||
			     posComp->position_.x - rectCollComp->size_.y/2.f > windowSize.x ||
			     posComp->position_.x + rectCollComp->size_.y/2.f < 0)))
			{
				entities_.erase(it);
			}

			else
			{
				++it;
			}
		}
	}
}


void World::draw() const
{
	for(auto it = entities_.begin(); it != entities_.end(); ++it)
	{
		window_.draw(*(*it));
	}
	window_.draw(difficulty_);
	drawFutureBalls();
}

void World::drawFutureBalls() const
{
	Vector2f firstBallPos {2.5f, 570.f};
	for (const auto& data : ballBuffer_)
	{
		sf::CircleShape circ (ballRadius_);
		circ.setFillColor(data.first.color);
		circ.setPosition(firstBallPos);
		firstBallPos.y -= 60.f;
		window_.draw(circ);
	}
}


