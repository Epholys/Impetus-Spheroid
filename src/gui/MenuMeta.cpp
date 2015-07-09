#include <algorithm>
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

	void MenuMeta::select()
	{
		Menu::select();
	}
	
	void MenuMeta::select(std::size_t index)
	{
		if(selectedChild_ >= 0)
		{
			buttons_[selectedChild_].button.setAlpha(150);
		}
		buttons_[index].button.setAlpha(255);
		Menu::select(index);
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
		assert(nButton);
		float length = (SIZE.x - SPACE*(nButton-1)) / nButton;
		Vector2f buttonSize (length, SIZE.y);
		
		ButtonEntry entry;
		entry.button = Button();
		entry.name = name;
		entry.menuIndex = children_.size() - 1; 
		buttons_.push_back(entry);
		
		for(int i=0; i<nButton; ++i)
		{
			sf::Texture texture;
			createButtonTexture(texture, buttonSize);
			buttons_[i].button.setTexture(texture);
			buttons_[i].button.setLabel(gui::Button::Middle, buttons_[i].name, font_, 30, sf::Color::Black);
			buttons_[i].button.setAlpha(150);
			buttons_[i].button.setParent(this);
			buttons_[i].button.setPosition(Vector2f());
			buttons_[i].button.move((length + SPACE) * i, 0.f);
			buttons_[i].button.setCallback([this, i](){this->select(i);});
		}
	}

	void MenuMeta::createButtonTexture(sf::Texture& texture, Vector2f size)
	{
		const sf::Color COLOR (230, 230, 230);
		
		sf::RenderTexture rTexture;
		assert(rTexture.create(size.x, size.y));

		rTexture.clear(COLOR);
		rTexture.display();
		texture = sf::Texture(rTexture.getTexture());
	}

	void MenuMeta::unpack(const std::string& name)
	{
		auto foundIt = std::find_if(buttons_.begin(),
		                            buttons_.end(),
		                            [name](const ButtonEntry& entry)
		                            {
			                            return entry.name == name;
		                            });
		if(foundIt != buttons_.end())
		{
			children_.erase(children_.begin() + foundIt->menuIndex);
			buttons_.erase(foundIt);
		}
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

		children_[selectedChild_]->handleEvent(event);
	}
	
	void MenuMeta::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		for(auto& entry : buttons_)
		{
			target.draw(entry.button, states);
		}

		target.draw(*children_[selectedChild_], states);
	}
}
