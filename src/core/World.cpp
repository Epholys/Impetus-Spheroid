#include "core/World.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------
// *** constructor: ***

World::World(sf::RenderWindow& window, int precision)
	: window_(window)
	, ecs_()
	, physEng_(ecs_, precision)
	, evtGen_()
	, difficulty_(DifficultyContext{this, &evtGen_})
	, state_(Waiting)
	, speedCoeff_(1.f)
	, entities_()
	, entitiesModifiers_()
	, ballType_(Ball::Normal)
	, gravityVect_(0.f, 1000.f)
	, ballBuffer_()
	, modifiers_()
	, modifierBuffer_()
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
	                              sf::Color::Blue));
	
	Entity::Ptr leftWall (new Wall(this, ecs_,
	                               Vector2f(0.f, winSize.y / 2.f),
	                               Vector2f(10.f, winSize.y),
	                               sf::Color::Blue));

	Entity::Ptr rightWall (new Wall(this, ecs_,
	                                Vector2f(winSize.x, winSize.y / 2.f),
	                                Vector2f(10.f, winSize.y),
	                                sf::Color::Blue));

	entities_.push_back(std::move(ceiling));
	entities_.push_back(std::move(leftWall));
	entities_.push_back(std::move(rightWall));
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

const eg::PhysicEngine& World::getPhysicEngine() const
{
	return physEng_;
}

void World::addEntityModifier(Modifier<Entity> modifier)
{
	entitiesModifiers_.push_back(modifier);
}

void World::addModifier(Modifier<World> modifier)
{
	modifierBuffer_.push_back(modifier);
}

void World::addEntity(Entity::Ptr entity)
{
	entities_.push_back(std::move(entity));
}

bool World::isGameOver() const
{
	return state_ == GameOver;
}

void World::setState(GameState state)
{
	state_ = state;
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
// *** TEMPORARY FUNCTIONS: ***
// TODO: Replace all the input by a separate class

//-----------------------------------------------------------------------------
// *** gameloop functions: ***
void World::createBall(Vector2f mousePosition)
{
	const Vector2f CANON_POSITION {20.f, 580.f};
	const float IMPULSE_COEFF = 3.f;

	BallData data;
	for(auto ritr = ballDatas.rbegin(); ritr != ballDatas.rend(); ++ritr)
	{
		int rint = randInt(1,1000);
		if(rint <= (*ritr).proba)
		{
			data = *ritr;
			break;
		}
	}

	Entity::Ptr pBall (new Ball(this, ecs_,
	                            CANON_POSITION,
	                            ballRadius_, ballMass_, gravityVect_, data, ballType_));


	auto velComp = dynCast<ecs::Velocity>
		(ecs_.getComponent(pBall->getLabel(), ecs::Component::Velocity));
	assert(velComp);

	Vector2f direction = mousePosition - CANON_POSITION;
	float impulse = std::max(direction.x, -direction.y);
	direction.normalize();
			

	velComp->velocity_ += IMPULSE_COEFF * impulse * direction / ballMass_;


	entities_.push_back(std::move(pBall));
}

void World::createTarget(Vector2f mousePosition)
{
	const Vector2f TARGET_DIMENSION {15.f, 75.f};
	const sf::Color TARGET_COLOR {sf::Color::Yellow};


	Entity::Ptr pTarget (new Target(this, ecs_,
	                                mousePosition,
	                                TARGET_DIMENSION,
	                                TARGET_COLOR));
	entities_.push_back(std::move(pTarget));

}

void World::handleInput(const sf::Event& event)
{
	if(state_ == Waiting || state_ == GameOver)
	{
		if(event.type == sf::Event::MouseButtonPressed)
			state_ = Playing;
		return;
	}

	if (event.type == sf::Event::KeyReleased)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::O:
			// Reset
			difficulty_.reset();
			entities_.clear();
			speedCoeff_ = 1.f;
			generateWorld();
			break;

		case sf::Keyboard::M:
			// Mask/UnMask
			difficulty_.mask();
			break;

		case sf::Keyboard::W:
			ballType_ ^= Ball::Massless;
			break;

		case sf::Keyboard::X:
		{
			ballType_ ^= Ball::Ghost;
			if(ballType_ & Ball::Ghost)
				ballColor_.a = 130;
			else
				ballColor_.a = 255;
			break;
		}

		default:
			break;
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed &&
	         event.mouseButton.button == sf::Mouse::Left)
	{
		createBall(Vector2f(sf::Mouse::getPosition(window_)));
	}

	else if (event.type == sf::Event::MouseButtonPressed &&
	         event.mouseButton.button == sf::Mouse::Right)			
	{
		createTarget(Vector2f(sf::Mouse::getPosition(window_)));
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

void World::cleanModifiers()
{
	entitiesModifiers_.clear();

	auto itEnd = std::remove_if(modifiers_.begin(),
	                            modifiers_.end(),
	                            [](const Modifier<World>& modifier)
	                            {
		                            return modifier.isExpired();
	                            });
	
	modifiers_.erase(itEnd, modifiers_.end());
}

void World::applyModifiers(Time dt)
{
	if(!modifierBuffer_.empty())
	{
		modifiers_.insert(modifiers_.begin(),
		                  modifierBuffer_.begin(),
		                  modifierBuffer_.end());
		modifierBuffer_.clear();
	}

	for(auto& modifier : modifiers_)
	{
		modifier(*this, dt);
	}

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
}

