#ifndef APPLICATION_HPP
#define APPLICATION_HPP


#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "utility/Vector2.hpp"
#include "framework/StateStack.hpp"
#include "framework/StateGame.hpp"
#include "framework/StateOver.hpp"
#include "framework/StatePause.hpp"
#include "framework/StateMarket.hpp"
#include "core/MetaData.hpp"

/* Application is the main class of the project. It is instanciated in main(),
 * Application.run() is then called and launch the game.
 *
 * WARNING:
 * Supervises window redimensioning *alone*: all the other class are in the
 * illusion of a WINDOW_SIZE (defined in the .cpp) sized window. For this
 * purpose, Application:
 *  - updates and forwards a globalTransform_
 *  - modifies window_'s Events to correct the coordinate of the mouse's
 * position
 * */

class Application
{
public:
	Application();

	void run();

private:
	void handleInput();

	// Update globalTransform_when window_ is resized.
	void handleResizing(Vector2u newWindowSize);

	void correctMouseCoordinate(Vector2i& mousePosition);

	void update(sf::Time dt);
	void render();

private:
	sf::RenderWindow window_;
	sf::Transform globalTransform_;
	MetaData metaData_;

	StateStack stack_;
};


#endif // APPLICATION_HPP
