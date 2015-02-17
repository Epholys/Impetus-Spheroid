#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <vector>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "framework/StateStack.hpp"
#include "ecs/EntityManager.hpp"
#include "engine/PhysicEngine.hpp"
#include "core/World.hpp"
#include "core/Ball.hpp"
#include "core/Wall.hpp"

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

	StateStack stack_;

	World gameWorld_;
};


#endif // APPLICATION_HPP
