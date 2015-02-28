template<typename T>
Modifier<T>::Modifier()
	: duration_()
	, firstTimeExecuted_(true)
	, lastTimeExecuted_(false)
	, preFunction_()
	, mainFunction_()
	, postFunction_()
	, preDelay_()
{
}

template<typename T>
bool Modifier<T>::isExpired() const
{
	return duration_ < Time();
}

template<typename T>
Time Modifier<T>::getTotalDuration() const
{
	return duration_ + preDelay_;
}

template<typename T>
void Modifier<T>::forceEnding(T& target, Time dt)
{
	if(postFunction_)
	{
		postFunction_(target, dt);
		lastTimeExecuted_ = true;		
	}
}

template<typename T>
void Modifier<T>::operator() (T& target, Time dt)
{
	if(preDelay_ > Time())
	{
		preDelay_ -= dt;
		return;
	}

	if(isExpired())
	{
		return;
	}
	
	if(firstTimeExecuted_)
	{
		if(preFunction_)
		{
			preFunction_(target, dt);
		}
		firstTimeExecuted_ = false;
	}

	if(mainFunction_)
	{
		mainFunction_(target, dt);
	}

	duration_ -= dt;

	if(isExpired() && postFunction_ && !lastTimeExecuted_)
	{
		postFunction_(target, dt);
		lastTimeExecuted_ = true;
	}
}

