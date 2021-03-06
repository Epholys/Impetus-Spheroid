#ifndef POWER_UP_TOOGLE_HPP
#define POWER_UP_TOOGLE_HPP


#include <functional>

#include "core/PowerUp.hpp"

class PowerUpToogle : public PowerUp
{
public:
	PowerUpToogle();
	virtual ~PowerUpToogle();

	virtual void apply (World& world);
	virtual PowerUpType::Type getType() const;

	void addActivateFunc(std::function<void(World&)> func);
	void addDeactivateFunc(std::function<void(World&)> func);

	bool isActivated() const;

private:
	std::function<void(World&)> activate_;
	std::function<void(World&)> deactivate_;
	bool isActivated_;
};


#endif
