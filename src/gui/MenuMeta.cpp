#include "gui/MenuMeta.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	namespace
	{
		const Vector2f SIZE (650, 50);
		const float SPACE = 5.f;
	}
	
	MenuMeta::MenuMeta(Menu::SelectionType type)
		: Menu(type)
		, font_()
		, buttons_()
	{
		font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");
	}

	MenuMeta::~MenuMeta()
	{
	}


//------------------------------------------------------------------------------	
	
	void MenuMeta::pack(Menu::SPtr menu, const std::string& name)
	{
		Menu::pack(menu);
		addButton(name);
	}

	void MenuMeta::addButton(const std::string& name)
	{
		int nButton = children_.size();
		float length = (SIZE.x - SPACE*(nButton-1)) / nButton;
		Vector2f buttonSize (length, SIZE.y);
		
		buttons_.push_back({Button(), name});
		
		for(int i=0; i<nButton; ++i)
		{
			sf::Texture texture;
			createButtonTexture(texture, buttonSize, buttons_[i].name);
			buttons_[i].button.setTexture(texture);
			buttons_[i].button.setParent(this);
			buttons_[i].button.setPosition(Vector2f());
			buttons_[i].button.move((length + SPACE) * i, 0.f);
		}
	}

	void MenuMeta::createButtonTexture(sf::Texture& texture, Vector2f size, const std::string& name)
	{
		const sf::Color COLOR (230, 230, 230);
		const sf::Color TEXT_COLOR = sf::Color::Black;
		
		sf::RenderTexture rTexture;
		assert(rTexture.create(size.x, size.y));

		sf::Text text(name, font_);
		text.setColor(TEXT_COLOR);
		centerOrigin(text);
		text.setPosition(size / 2.f);

		rTexture.clear(COLOR);
		rTexture.draw(text);
		rTexture.display();
		texture = sf::Texture(rTexture.getTexture());
	}
	
//------------------------------------------------------------------------------

	void MenuMeta::handleEvent(const sf::Event& event)
	{
		if(event.type == sf::Event::MouseButtonPressed ||
		   event.type == sf::Event::MouseButtonReleased ||
		   event.type == sf::Event::MouseMoved)
		{
			for(auto& entry : buttons_)
			{
				entry.button.handleEvent(event);
			}
		}

		Menu::handleEvent(event);
	}
	
	void MenuMeta::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		for(auto& entry : buttons_)
		{
			target.draw(entry.button, states);
		}

		for (const auto& child : children_)
	    {
		    target.draw(*child, states);
	    }
	}
}
