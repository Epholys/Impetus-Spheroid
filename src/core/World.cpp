#include "core/World.hpp"


//-----------------------------------------------------------------------------
// *** constructor: ***

World::World(ecs::EntityManager& ecs, sf::RenderWindow& window, int precision)
	: ecs_(ecs)
	, window_(window)
	, physEng_(ecs, precision)
	, entities_()
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
				ecs_.pauseAllComponents(ecs::Component::Velocity, sf::seconds(2));
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
			
			default:
				break;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Left)
		{
			Entity::Ptr pBall (new Ball(ecs_,
			                            Vector2f(20.f, 580.f),
			                            ballRadius_, ballMass_, ballColor_));


			auto velComp = dynCast<ecs::Velocity>
				(ecs_.getComponent(pBall->getLabel(), ecs::Component::Velocity));
			assert(velComp);

			Vector2f force = Vector2f(20.f, 580.f) - Vector2f(sf::Mouse::getPosition(window_));

			velComp->velocity_ -= 3.f * force / ballMass_;


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
	physEng_.update(dt);

	for(const auto& ent : entities_)
	{
		ent->update(dt);
	}

	cleanEntities();

	score_ += physEng_.getTrackedCollisions().size();
	std::stringstream ss;
	ss << score_;
	scoreText_.setString(ss.str());

}

void World::cleanEntities()
{
	auto it = entities_.begin();
	while(it != entities_.end())
	{
		auto posPtr = ecs_.getComponent((*(it))->getLabel(),
		                                ecs::Component::Position);
		auto collPtr = ecs_.getComponent((*(it))->getLabel(),
		                                 ecs::Component::Collidable);
		if(posPtr && collPtr)
		{
			bool erase = false;

			sf::Vector2f windowSize (window_.getSize());

			auto posComp = dynCast<ecs::Position>(posPtr);
			assert(posComp);


			auto sphereCollComp = dynCast<ecs::CollidableSphere>(collPtr);
			auto rectCollComp = dynCast<ecs::CollidableRect>(collPtr);

			if((sphereCollComp &&
			    posComp->position_.y + sphereCollComp->radius_ > windowSize.y) ||
			   (rectCollComp &&
			    posComp->position_.y + rectCollComp->size_.y/2.f > windowSize.y))
			{
				erase = true;
			}

			if(erase)
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
	for(const auto& ent : entities_)
	{
		window_.draw(*ent);
	}

	window_.draw(scoreText_);
}

