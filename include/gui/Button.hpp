#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP


#include <functional>
#include <memory>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "gui/Component.hpp"

namespace gui
{ 
	class Button : public Component
	{
	public:
		typedef std::shared_ptr<Button> SPtr;
		typedef std::function<void()> Callback;

	public:
		Button();
		virtual ~Button();

		void setCallback(Callback callback);
		void setTexture(sf::Texture texture);
		void setAlpha(sf::Uint8 alpha);
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
	};
}


#endif // GUI_BUTTON_HPP
