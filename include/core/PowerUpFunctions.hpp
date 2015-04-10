#ifndef POWER_UP_FUNCTIONS_HPP
#define POWER_UP_FUNCTIONS_HPP

#include <map>
#include <memory>

#include <SFML/Graphics/Texture.hpp>

#include "core/PowerUp.hpp"
#include "core/Identifiers.hpp"
#include "core/World.hpp"


void genPowerUps(std::map<PowerUpID::ID, std::shared_ptr<PowerUp>>& powerUps,
                 std::map<PowerUpID::ID, int>& numbers,
                 std::map<PowerUpID::ID, sf::Texture>& textures);


#endif // POWER_UP_FUNCTIONS_HPP
