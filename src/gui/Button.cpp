#include <SFML/Graphics/RenderTarget.hpp>
#include "gui/Button.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	Button::Button()
		: Component()
		, callback_()
		, shortcut_(sf::Keyboard::KeyCount)
		, sprite_()
		, texture_()
	{
	}

	Button::~Button()
	{
	}



//-----------------------------------------------------------------------------

	void Button::setCallback(Callback callback)
	{
		callback_ = callback;
	}

	void Button::setTexture(sf::Texture texture)
	{
		texture_ = texture;
		sprite_.setTexture(texture_);
	}

	void Button::setKey(sf::Keyboard::Key key)
	{
		shortcut_ = key;
	}


//-----------------------------------------------------------------------------

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		Component::select();
		sprite_.setColor(sf::Color(255, 225, 225));
	}

	void Button::deselect()
	{
		Component::deselect();
		sprite_.setColor(sf::Color(255, 255, 255));
	}

	void Button::activate()
	{
		Component::activate();
		sprite_.setColor(sf::Color(255, 200, 200));
	}

	void Button::deactivate()
	{
		Component::deactivate();
		sprite_.setColor(sf::Color(255, 255, 255));

		if(callback_)
		{
			callback_();
		}
	}
	

//-----------------------------------------------------------------------------

	void Button::handleEvent(const sf::Event& event)
	{
		sf::FloatRect spriteRect = sprite_.getGlobalBounds();
		spriteRect = getTransform().transformRect(spriteRect);
		bool isMouseOver = false;

		if(event.type == sf::Event::MouseMoved)
		{
			auto mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
			isMouseOver = spriteRect.contains(mousePos.x, mousePos.y);

			if(isMouseOver)
			   select();
			else
			   deselect();
		}
		if(event.type == sf::Event::MouseButtonPressed &&
		   event.mouseButton.button == sf::Mouse::Left)
		{
			auto mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
			isMouseOver = spriteRect.contains(mousePos.x, mousePos.y);

			if(isMouseOver)
				activate();
		}
		if(event.type == sf::Event::MouseButtonReleased &&
		   event.mouseButton.button == sf::Mouse::Left)
		{
			auto mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
			isMouseOver = spriteRect.contains(mousePos.x, mousePos.y);
			
			if(isMouseOver)
			{
				deactivate();
				select();
			}
		}
	
		if(event.type == sf::Event::KeyPressed &&
		        event.key.code == shortcut_)
		{
			activate();
		}
		if(event.type == sf::Event::KeyReleased &&
		        event.key.code == shortcut_)
		{
			deactivate();
		}
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(sprite_, states);
 	}
}
