#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP


#include <string>
#include <functional>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "utility/Vector2.hpp"
#include "gui/Component.hpp"

namespace gui
{
	template <typename T>
	class Slider : public Component
	{
	public:
		typedef T Type;
		typedef std::shared_ptr<Slider> SPtr;
		typedef std::function<void(T&)> Operation;

	public:
		Slider();
		Slider(T& var,
		       Vector2f size,
		       const sf::Font& font,
			   const std::string& name);
		
		void setName(const std::string& name);
		void updateText();

		void setOperationPlus(Operation op);
		void setOperationMinus(Operation op);

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();

		virtual void handleEvent(const sf::Event& event);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		T& variable_;
		Operation opPlus_;
		Operation opMinus_;
	
		sf::RectangleShape backRectangle_;
		sf::Text varValue_;
		sf::Text varName_;
	};

	template<typename T>
	std::function<void(T&)> defaultOpPlus(const T& incr, const T& max)
	{
		return
			[&](T& t)
			{
				if(t + incr < max)
					t += incr;
			};
	}

	template<typename T>
	std::function<void(T&)> defaultOpMinus(const T& incr, const T& min)
	{
		return
			[&](T& t)
			{
				if(t - incr > min)
					t -= incr;
			};
	}
}

#include "Slider.inl"

#endif // GUI_SLIDER_HPP
