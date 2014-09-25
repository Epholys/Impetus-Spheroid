#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <vector>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "ecs/EntityManager.hpp"
#include "engine/PhysicEngine.hpp"
#include "Ball.hpp"

/* The main object: the framework for everything else
 * 
 * Manages the sf::RenderWindow and some basic sf::Events.
 * Contain the simple game loop in run().
 * */

class Application
{
public:
	Application();

	void run();

private:
	void handleInput();
	void update(sf::Time dt);
	void render();

private:
	sf::RenderWindow window_;
	ecs::EntityManager ecs_;
	eg::PhysicEngine engine_;

	std::vector<std::unique_ptr<Ball>> balls_;
};


#endif // APPLICATION_HPP
