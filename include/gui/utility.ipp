
template<typename T>
void FadeOut<T>::update(Time dt)
{
	if(!pT_)
		return;
		
	if(accumulatedTime_ >= duration_)
	{
		pT_->setColor(sf::Color(0,0,0,0));
		return;
	}

	accumulatedTime_ += dt;

	sf::Color color = pT_->getColor();
	float a = std::max(0.f, 255 - (255 / duration_.asSeconds()) * accumulatedTime_.asSeconds());
	color.a = static_cast<sf::Uint8>(a);
	pT_->setColor(color);
}

