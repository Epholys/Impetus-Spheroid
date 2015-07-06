#ifndef MARKET_HPP
#define MARKET_HPP


#include <functional>
#include <sstream>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include "framework/State.hpp"
#include "framework/DataSaver.hpp"
#include "gui/Menu.hpp"
#include "gui/Button.hpp"
#include "gui/TextureGenerator.hpp"
#include "gui/utility.hpp"
#include "gui/Transition.hpp"
#include "core/Inventory.hpp"
#include "data/MarketData.hpp"

class Market : public sf::Drawable
{
public:
	Market(State::Context context);

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(Time dt);
	
private:
	void initText(sf::Font& font);
	void initGUI(sf::Font& font);

	void buy(PowerUpID::ID id, int number, int price);

	void updateCoinsText();
	void updateCoinsLoss(int price);

	void createBlankButton(sf::Texture& texture, int price, sf::Font& font);

private:
	Inventory& inventory_;
	std::vector<bool>& isUnlocked_;
	
	gui::Menu::SPtr menu_;
	std::map<PowerUpID::ID, gui::Button::SPtr> buttons_;
	sf::Text coinsText_;

	sf::Text coinsLost_;
	gui::Transition coinsLostTransition_;
	gui::FadeOut<sf::Text> coinsLostFadeOut_;
	
	State::Context context_;
};


#endif // MARKET_HPP
