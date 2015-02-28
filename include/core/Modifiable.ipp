
template<typename T>
void Modifiable<T>::addModifier(Modifier<T> modifier)
{
	modifierBuffer_.push_back(modifier);
}

template<typename T>
void Modifiable<T>::cleanModifiers()
{
	auto itEnd =
		std::remove_if(modifiers_.begin(),
		               modifiers_.end(),
		               [](const Modifier<T>& modEnt)
		               {
			               return modEnt.isExpired();
		               });
	
	modifiers_.erase(itEnd, modifiers_.end());
}

template<typename T>
void Modifiable<T>::applyModifiers(T& t, Time dt)
{
	if(!modifierBuffer_.empty())
	{
		modifiers_.insert(modifiers_.begin(),
		                  modifierBuffer_.begin(),
		                  modifierBuffer_.end());
		modifierBuffer_.clear();
	}

	for(auto& modifier : modifiers_)
	{
		modifier(t, dt);
	}
}

template<typename T>
void Modifiable<T>::forceEndingModifiers(T& t, Time dt)
{
	for(auto modifier : modifiers_)
	{
		modifier.forceEnding(t, dt);
	}
}
