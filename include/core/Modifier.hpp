#ifndef MODIFIER_HPP
#define MODIFIER_HPP


#include <functional>

#include "utility/Time.hpp"
#include "ecs/EntityManager.hpp"

template <typename T>
class Modifier
{
public:
	Modifier();

	bool isExpired() const;

	void operator() (T& target, Time dt);

public:
	Time duration_;
	bool firstTimeExecuted_;

	std::function<void(T&, Time dt)> preFunction_;
	std::function<void(T&, Time dt)> mainFunction_;
	std::function<void(T&, Time dt)> postFunction_;
};

#include "Modifier.ipp"


#endif // MODIFIER_HPP
