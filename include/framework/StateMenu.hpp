#ifndef STATE_MENU_HPP
#define STATE_MENU_HPP


#include <string>

#include "framework/State.hpp"
#include "framework/DataSaver.hpp"
#include "gui/Menu.hpp"
#include "gui/Button.hpp"

// The Menu State, when the player hit "Escape"

class StateMenu : public State
{
public:
	StateMenu(StateStack& stack, Context context);
	virtual ~StateMenu();

	virtual void draw(sf::RenderStates states);
	virtual bool update(Time dt);
	virtual bool handleInput(const sf::Event& event);

private:
	gui::Button::SPtr genStandardButton(const std::string& label) const;
	
private:
	gui::Menu menu_;
	sf::Sprite logo_;
};


#endif // STATE_MENU_HPP
