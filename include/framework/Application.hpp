#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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
};


#endif // APPLICATION_HPP
