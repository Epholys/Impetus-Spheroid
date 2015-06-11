#ifndef POWER_UP_FUNCTIONS_HPP
#define POWER_UP_FUNCTIONS_HPP

#include <map>
#include <memory>

#include <SFML/Graphics/Texture.hpp>

#include "core/PowerUp.hpp"
#include "core/Identifiers.hpp"
#include "core/World.hpp"

struct PowerUpEntry;

void genPowerUps(std::map<PowerUpID::ID, PowerUpEntry>& powerUpTable);

#endif // POWER_UP_FUNCTIONS_HPP
