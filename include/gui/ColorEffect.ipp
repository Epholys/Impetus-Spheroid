template<typename T>
ColorEffect<T>::ColorEffect(T* toColor,
                            ColorEffectID::ID type,
                            sf::Color start,
                            sf::Color finish,
                            Time duration)
	: toColor_(toColor)
	, type_(type)
	, start_(start)
	, finish_(finish)
	, duration_(duration)
	, accumulatedTime_()
	, effectFunction_(generateColorEffectFunction(type_, start_, finish_, duration_))
{
	if(toColor)
	{
		toColor->setColor(start);
	}
}

template<typename T>
void ColorEffect<T>::update(Time dt)
{
	if(accumulatedTime_ >= duration_ || !effectFunction_ || !toColor_)
		return;

	accumulatedTime_ += dt;

	if(accumulatedTime_ >= duration_)
	{
		// Not really generic, just used by SFML and I so this is by default
		toColor_->setColor(finish_);
	}
	else
	{
		// The same goes for this
		toColor_->setColor(effectFunction_(accumulatedTime_));
	}
}

template<typename T>
void ColorEffect<T>::updateFinish(sf::Color newFinish, Time toAdd)
{
	finish_ = newFinish;
 	duration_ += toAdd;
	effectFunction_ = generateColorEffectFunction(type_, start_, finish_, duration_);
}

template<typename T>
bool ColorEffect<T>::isOver() const
{
	return accumulatedTime_ >= duration_;
}

