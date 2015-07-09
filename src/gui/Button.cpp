#include <SFML/Graphics/RenderTarget.hpp>
#include "gui/Button.hpp"


//-----------------------------------------------------------------------------

namespace gui
{

	namespace
	{
		sf::Texture defaultTexture;
		void initDefaultTexture (sf::Texture& texture)
		{
			texture.loadFromFile("./media/sprites/Blankbutton.png");
		}
	}
	
//-----------------------------------------------------------------------------
   
	Button::Button()
		: Component()
		, callback_()
		, shortcut_(sf::Keyboard::KeyCount)
		, sprite_()
		, texture_()
		, alpha_(255)
		, labels_(5)
	{
		initDefaultTexture(defaultTexture);
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
		sprite_.setTexture(texture_, true);
	}

	void Button::setDefaultTexture()
	{
		setTexture(defaultTexture);
	}

	void Button::setAlpha(sf::Uint8 alpha)
	{
		alpha_ = alpha;
		sf::Color color = sprite_.getColor();
		color.a = alpha_;
		sprite_.setColor(color);
	}

	void Button::setLabel(LabelPosition position,
	                      const std::string& string,
	                      const sf::Font& font,
	                      unsigned int characterSize,
	                      sf::Color color,
	                      Vector2f toMove)
	{
		sf::Text& text = labels_[position];
	    text = sf::Text(string, font, characterSize);
		text.setColor(color);
		Vector2f textureSize = Vector2f(texture_.getSize());
		sf::FloatRect textRect = text.getGlobalBounds();
		centerOrigin(text);
		switch(position)
		{
		case Middle:
			text.move(textureSize / 2.f);
			break;
		case Up:
			text.move(textureSize.x / 2.f, -textRect.height / 2.f);
			break;
		case Right:
			text.move(textureSize.x + textRect.width / 2.f, textureSize.y / 2.f);
			break;
		case Bottom:
			text.move(textureSize.x / 2.f, textureSize.y + textRect.height / 2.f);
			break;
		case Left:
			text.move(-textRect.width / 2.f, textureSize.y / 2.f);
			break;
		}
		text.move(toMove);
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
		sprite_.setColor(sf::Color(255, 225, 225, alpha_));
	}

	void Button::deselect()
	{
		Component::deselect();
		sprite_.setColor(sf::Color(255, 255, 255, alpha_));
	}

	void Button::activate()
	{
		Component::activate();
		sprite_.setColor(sf::Color(255, 200, 200, alpha_));
	}

	void Button::deactivate()
	{
		Component::deactivate();
		sf::Color color = (isSelected()) ? sf::Color(255, 225, 225, alpha_) : sf::Color(255, 255, 255, alpha_);
		sprite_.setColor(color);
		if(callback_)
		{
			callback_();
		}
	}
	

//-----------------------------------------------------------------------------

	void Button::handleEvent(const sf::Event& event)
	{
		sf::FloatRect spriteRect = sprite_.getLocalBounds();
		spriteRect = getGlobalTransform().transformRect(spriteRect);
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
		for(const auto& text : labels_)
		{
			target.draw(text, states);
		}
 	}
}
