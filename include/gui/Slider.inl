#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "gui/Slider.hpp"
#include "utility/utility.hpp"


namespace gui
{
	template <typename T>
	Slider<T>::Slider()
		: variable_()
		, opPlus_()
		, opMinus_()
		, backRectangle_()
		, varValue_()
		, varName_()
	{
	}


	// Some magic variable here, but Sliders are for quick adjustment in
	// prototype phase only
	template <typename T>
	Slider<T>::Slider(T& var,
	                  Vector2f size,
	                  const sf::Font& font,
					  const std::string& name)
		: variable_(var)
		, opPlus_()
		, opMinus_()
		, backRectangle_(size)
		, varValue_(toString(var), font, 20)
		, varName_(name, font, 20)
	{
		setOperationPlus([](T& t){t += 1;});
		setOperationMinus([](T& t){t -= 1;});

		backRectangle_.setFillColor(sf::Color(0, 0, 0, 150));
		

		sf::Vector2f rectSize = backRectangle_.getSize();

		centerOrigin(varValue_);
		varValue_.move(rectSize.x / 2.f, 0.f);
	   
		varName_.setColor(sf::Color::Black);
		varName_.move(rectSize.x + 5.f, -5.f);
	}

//------------------------------------------------------------------------------

	template<typename T>
	bool Slider<T>::isSelectable() const
	{
		return true;
	}

	template<typename T>
	void Slider<T>::select()
	{
		Component::select();
		backRectangle_.setFillColor(sf::Color(0, 0, 0, 200));
	}

	template<typename T>	   
    void Slider<T>::deselect()
	{
		Component::deselect();
		backRectangle_.setFillColor(sf::Color(0, 0, 0, 150));
	}

	template<typename T>
	void Slider<T>::handleEvent(const sf::Event& event)
	{
		if (!isSelected())
			return;

		if(event.type == sf::Event::KeyPressed)
		{
			switch(event.key.code)
			{
			case sf::Keyboard::Right:
				opPlus_(variable_);
				break;

			case sf::Keyboard::Left:
				opMinus_(variable_);
				break;

			default:
				break;
			}

			varValue_.setString(toString(variable_));
		}
	}


//------------------------------------------------------------------------------

	template<typename T>
	void Slider<T>::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(backRectangle_, states);
		target.draw(varValue_, states);
		target.draw(varName_, states);
	}


//------------------------------------------------------------------------------

	template<typename T>
	void Slider<T>::setName(const std::string& name)
	{
		varName_.setString(name);
	}

	template<typename T>
	void Slider<T>::updateText()
	{
		varValue_.setString(toString(variable_));
	}

	template<typename T>
	void Slider<T>::setOperationPlus(Operation op)
	{
		opPlus_ = std::move(op);
	}

	template<typename T>
	void Slider<T>::setOperationMinus(Operation op)
	{
		opMinus_ = std::move(op);
	}

} // namespace GUI
