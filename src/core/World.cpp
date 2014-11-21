#include "core/World.hpp"


//-----------------------------------------------------------------------------
// *** constructor: ***

World::World(ecs::EntityManager& ecs, sf::RenderWindow& window, int precision)
	: ecs_(ecs)
	, window_(window)
	, physEng_(ecs, precision)
	, entities_()
	, entitiesModifiers_()
	, ballType_(Ball::Normal)
	, modifiers_()
	, font_()
	, score_(0)
	, scoreText_()
	, ballMass_(1.f)
	, ballRadius_(10.f)
	, ballColor_(sf::Color::Red)
{
	generateWorld();

	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");
	scoreText_.setString("0");
	scoreText_.setFont(font_);
}

void World::generateWorld()
{
	auto winSize = window_.getSize();

	Entity::Ptr ceiling (new Wall(ecs_,
	                              Vector2f(winSize.x / 2.f, 0.f),
	                              Vector2f(winSize.x, 10.f),
	                              sf::Color::Blue));
	
	Entity::Ptr leftWall (new Wall(ecs_,
	                               Vector2f(0.f, winSize.y / 2.f),
	                               Vector2f(10.f, winSize.y),
	                               sf::Color::Blue));

	Entity::Ptr rightWall (new Wall(ecs_,
	                                Vector2f(winSize.x, winSize.y / 2.f),
	                                Vector2f(10.f, winSize.y),
	                                sf::Color::Blue));

	entities_.push_back(std::move(ceiling));
	entities_.push_back(std::move(leftWall));
	entities_.push_back(std::move(rightWall));
}


//-----------------------------------------------------------------------------
// *** gameloop functions: ***

void World::handleInput()
{
	sf::Event event;
	while(window_.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window_.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				ecs_.pauseAllComponents(ecs::Component::Velocity, seconds(2));
				break;
				
			case sf::Keyboard::Add:
				ballMass_ *= 2.f;
				ballRadius_ *= 1.5f;
				break;

			case sf::Keyboard::Subtract:
				ballMass_ /= 2.f;
				ballRadius_ /= 1.5f;
				break;

			case sf::Keyboard::G:
				ballColor_ = (ballColor_ == sf::Color::Green) ? sf::Color::Red : sf::Color::Green;
				break;
				
			case sf::Keyboard::R:
				entities_.clear();
				generateWorld();
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

			case sf::Keyboard::A:
			{
				Modifier<Entity> chgGrav;
				chgGrav.duration_ = seconds(5);
				chgGrav.preFunction_ =
					[](Entity& target, Time)
					{
						auto massPtr = target.getComponents()[ecs::Component::Mass];
						auto massComp = dynCast<ecs::Mass>(massPtr);
						if(massComp)
						{
							massComp->gravityVect_ *= -1.f;
						}
					};
			
				chgGrav.postFunction_ =
					[](Entity& target, Time)
					{
						auto massPtr = target.getComponents()[ecs::Component::Mass];
						auto massComp = dynCast<ecs::Mass>(massPtr);
						if(massComp)
						{
							massComp->gravityVect_ *= -1.f;
						}
					};


				auto pStopTime = std::make_shared<Modifier<Entity>>();

				pStopTime->preDelay_ = seconds(0.5f);

				pStopTime->preFunction_ =
					[](Entity& target, Time)
					{
						if(target.getType() == EntityID::Ball)
						{
							auto velPtr = target.getComponents()[ecs::Component::Velocity];
							if(velPtr) velPtr->pause(seconds(3));
						}
					};
				
				chgGrav.successor_ = pStopTime;

				chgGrav.updateDuration();
				
				entitiesModifiers_.push_back(chgGrav);
				break;
			}
			
			default:
				break;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Left)
		{
			Entity::Ptr pBall (new Ball(ecs_,
			                            Vector2f(20.f, 580.f),
			                            ballRadius_, ballMass_, ballColor_, ballType_));


			auto velComp = dynCast<ecs::Velocity>
				(ecs_.getComponent(pBall->getLabel(), ecs::Component::Velocity));
			assert(velComp);

			Vector2f force = Vector2f(sf::Mouse::getPosition(window_)) - Vector2f(20.f, 580.f);
			float impulse = std::max(force.x, -force.y);
			force.normalize();
			

			velComp->velocity_ += 3.f * impulse * force / ballMass_;


			entities_.push_back(std::move(pBall));
		}

		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Right)			
		{
			Entity::Ptr pTarget (new Target(ecs_,
			                                Vector2f(sf::Mouse::getPosition(window_)),
			                                Vector2f(15.f, 75.f),
			                                sf::Color::Yellow));
			entities_.push_back(std::move(pTarget));
		}
	}
}

void World::update(Time dt)
{
	applyModifiers(dt);
	for(const auto& ent : entities_)
	{
		ent->update(dt);
	}
	physEng_.update(dt);

	cleanEntities();

	score_ += physEng_.getTrackedCollisions().size();
	std::stringstream ss;
	ss << score_;
	scoreText_.setString(ss.str());
}


void World::applyModifiers(Time dt)
{
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
	entitiesModifiers_.clear();
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
			    posComp->position_.y - sphereCollComp->radius_ > windowSize.y) ||
			   (rectCollComp &&
			    posComp->position_.y - rectCollComp->size_.y/2.f > windowSize.y))
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
	for(auto rit = entities_.rbegin(); rit != entities_.rend(); ++rit)
	{
		window_.draw(*(*rit));
	}

	window_.draw(scoreText_);
}

