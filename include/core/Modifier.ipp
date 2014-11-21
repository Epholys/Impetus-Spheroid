template<typename T>
Modifier<T>::Modifier()
	: duration_()
	, firstTimeExecuted_(true)
	, preFunction_()
	, mainFunction_()
	, postFunction_()
	, successor_(nullptr)
	, preDelay_()
{
}

template<typename T>
bool Modifier<T>::isExpired() const
{
	return duration_ < Time();
}

template<typename T>
void Modifier<T>::updateDuration()
{
	duration_ += preDelay_;

	auto succ = successor_.get();
	while (succ != nullptr)
	{
		duration_ += succ->duration_ + succ->preDelay_;
		succ = succ->successor_.get();
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

	if(isExpired())
	{
		if(postFunction_)
		{
			postFunction_(target, dt);
		}

		if(successor_)
		{
			duration_ = successor_->duration_;
			firstTimeExecuted_ = successor_->firstTimeExecuted_;
			preFunction_ = successor_->preFunction_;
			mainFunction_ = successor_->mainFunction_;
			postFunction_ = successor_->postFunction_;
			preDelay_ = successor_->preDelay_;
			successor_ = successor_->successor_;
		}
	}
}
