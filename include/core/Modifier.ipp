template<typename T>
Modifier<T>::Modifier()
	: duration_()
	, firstTimeExecuted_(true)
	, preFunction_()
	, mainFunction_()
	, postFunction_()
{
}

template<typename T>
bool Modifier<T>::isExpired() const
{
	return duration_ < Time();
}


template<typename T>
void Modifier<T>::operator() (T& target, Time dt)
{
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

	if(isExpired() && postFunction_)
	{
		postFunction_(target, dt);
	}
}
