#include "core/World.hpp"


//-----------------------------------------------------------------------------
// *** constructor: ***

World::World(ecs::EntityManager& ecs, sf::RenderWindow& window, int precision)
	: ecs_(ecs)
	, window_(window)
	, physEng_(ecs, precision)
	, entities_()
{
	generateWorld();
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

void World::handleInput(sf::Event)
{

}

void World::update(Time dt)
{
	physEng_.update(dt);
}

void World::cleanEntities()
{
}

void World::draw() const
{
	for(const auto& ent : entities_)
	{
		window_.draw(*ent);
	}
}
