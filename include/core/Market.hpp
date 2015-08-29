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
#include "gui/MenuMeta.hpp"
#include "gui/Button.hpp"
#include "gui/TextureGenerator.hpp"
#include "gui/utility.hpp"
#include "gui/Transition.hpp"
#include "core/Inventory.hpp"
#include "data/MarketData.hpp"
#include "data/ImprovementData.hpp"

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
	void initPowerUps(std::vector<gui::Menu::SPtr>& menu, sf::Font& font);
	void initImprovement(gui::Menu::SPtr menu, sf::Font& font);

	void buyPowerUp(PowerUpID::ID id, int number, int price);
	void buyImprovement(ImprovementID::ID id, int increment, int price);

	void updateCoinsText();
	void updateCoinsLoss(int price);

private:
	Inventory& inventory_;
	std::vector<bool>& isUnlocked_;
	std::vector<int>& improvementValue_;
	bool needUpdate_;
	
	gui::MenuMeta::SPtr menu_;
	std::map<PowerUpID::ID, gui::Button::SPtr> powerUpButtons_;
	std::map<ImprovementID::ID, gui::Button::SPtr> improvementButtons_;
	sf::Text coinsText_;

	sf::Text coinsLost_;
	gui::Transition coinsLostTransition_;
	gui::FadeOut<sf::Text> coinsLostFadeOut_;
	
	State::Context context_;
};


#endif // MARKET_HPP
