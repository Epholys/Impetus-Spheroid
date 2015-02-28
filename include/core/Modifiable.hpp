#ifndef MODIFIABLE_HPP
#define MODIFIABLE_HPP


#include <vector>

#include "utility/Time.hpp"
#include "core/Modifier.hpp"

template<typename T>
class Modifiable
{
public:
	virtual void addModifier(Modifier<T> modifier);
	virtual void cleanModifiers();
	virtual void applyModifiers(T& t, Time dt);
	virtual void forceEndingModifiers(T& t, Time dt = Time());

protected:
	std::vector<Modifier<T>> modifiers_;
	// The buffer is necessary: a Modifier can add others Modifier.
	std::vector<Modifier<T>> modifierBuffer_;
};

#include "core/Modifiable.ipp"


#endif // MODIFIABLE_HPP
