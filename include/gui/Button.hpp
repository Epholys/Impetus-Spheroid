#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP


#include <functional>
#include <memory>
#include <vector>
#include <string>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>

#include "utility/Vector2.hpp"
#include "utility/utility.hpp"
#include "gui/Component.hpp"

namespace gui
{ 
	class Button : public Component
	{
	public:
		typedef std::shared_ptr<Button> SPtr;
		typedef std::function<void()> Callback;

	public:
		enum LabelPosition
		{
			Middle,
			Up,
			Right,
			Bottom,
			Left
		};
		
	public:
		Button();
		virtual ~Button();

		void setCallback(Callback callback);

		void setTexture(sf::Texture texture);
		void setDefaultTexture();
		void setAlpha(sf::Uint8 alpha);

		void setLabel(LabelPosition position,
		              const sf::Text& text);
		void setLabel(LabelPosition position,
		              const std::string& string,
		              const sf::Font& font,
		              unsigned int characterSize = 30,
		              sf::Color color = sf::Color::White,
		              Vector2f toMove = Vector2f());
		sf::Text& getLabel(LabelPosition position);
		
		void removeLabel(LabelPosition position);
		void setKey(sf::Keyboard::Key key);
		

		virtual bool isSelectable() const;
		virtual void select();
		virtual void deselect();

		virtual void activate();
		virtual void deactivate();

		virtual void handleEvent(const sf::Event& event);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		Callback callback_;
		sf::Keyboard::Key shortcut_;

		sf::Sprite sprite_;
		sf::Texture texture_;
		sf::Uint8 alpha_;

		std::vector<sf::Text> labels_;
	};
}


#endif // GUI_BUTTON_HPP
