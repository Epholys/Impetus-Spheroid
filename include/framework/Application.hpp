#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <vector>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "framework/StateStack.hpp"
#include "framework/StateGame.hpp"
#include "framework/StateOver.hpp"
#include "framework/StatePause.hpp"
#include "ecs/EntityManager.hpp"
#include "engine/PhysicEngine.hpp"
#include "core/Inventory.hpp"

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
	Inventory inventory_;

	StateStack stack_;
};


#endif // APPLICATION_HPP
