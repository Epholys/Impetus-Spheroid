#ifndef INVENTORY_HPP
#define INVENTORY_HPP


#include <map>
#include <tuple>
#include <memory>
#include <algorithm>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "utility/utility.hpp"
#include "core/World.hpp"
#include "core/PowerUp.hpp"
#include "core/PowerUpToogle.hpp"
#include "core/PowerUpFunctions.hpp"

struct PowerUpEntry
{
	sf::Keyboard::Key key;
	std::shared_ptr<PowerUp> powerUp;
	int stock;
	sf::Texture texture;
};

class Inventory : public sf::NonCopyable,
                  public sf::Drawable, public sf::Transformable
{
public:
	Inventory();
	Inventory(Vector2u originalSize,
	          bool isAzerty);
	virtual ~Inventory();

	void switchKeyboard(bool isAzerty);
	void initKeyBinding(bool isAzerty);

	void addWorld(World* world);
	void removeWorld();

	void handleInput(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool setPowerUp(PowerUpID::ID id, int value);
	// Warning : return -1 if ID isn't valid.
	int getPowerUp(PowerUpID::ID id) const;
	bool decrement(PowerUpID::ID id);
	void increment(PowerUpID::ID id, int value);

	void addCoins(int n);
	bool removeCoins(int n);
	int getCoins() const;

    std::map<PowerUpID::ID, sf::Keyboard::Key> getKeys() const;

private:
	const Vector2f originalSize_;

	sf::Font font_;
	mutable sf::Text coinsText_;

	int coins_;

	std::map<sf::Keyboard::Key, PowerUpID::ID> keyBindings_;

	std::map<PowerUpID::ID, PowerUpEntry> powerUpTable_;

	World* world_;
};


#endif // INVENTORY_HPP
