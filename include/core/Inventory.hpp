#ifndef INVENTORY_HPP
#define INVENTORY_HPP


#include <map>
#include <tuple>
#include <memory>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "core/Identifiers.hpp"
#include "core/World.hpp"
#include "core/PowerUp.hpp"


class Inventory : public sf::NonCopyable,
                  public sf::Drawable, public sf::Transformable
{
public:
	Inventory(bool isAzerty = false);
	virtual ~Inventory();

	void initKeyBinding(bool isAzerty);
	void addWorld(World* world);
	void removeWorld();

	void handleInput(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool decrement(PowerUpID::ID id);
	void increment(PowerUpID::ID id, int value);

private:
	std::map<sf::Keyboard::Key, PowerUpID::ID> keyBindings_;
	std::map<PowerUpID::ID, std::tuple<std::shared_ptr<PowerUp>, int>> powerUps_;

	World* world_;
};


#endif // INVENTORY_HPP
