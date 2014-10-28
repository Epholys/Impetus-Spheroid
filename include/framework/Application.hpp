#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <vector>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ecs/EntityManager.hpp"
#include "engine/PhysicEngine.hpp"
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
	ecs::EntityManager ecs_;
	eg::PhysicEngine engine_;

	// Temporaries attributes to test PhysicEngine and EntityManager
	sf::Font font_;
	int score_;
	sf::Text scoreText_;

	std::vector<std::unique_ptr<Ball>> balls_;
	float ballMass_;
	float ballRadius_;
	sf::Color ballColor_;
	
	std::vector<std::unique_ptr<Wall>> rects_;
};


#endif // APPLICATION_HPP
