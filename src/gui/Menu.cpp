#include "gui/Menu.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace gui
{

//-----------------------------------------------------------------------------
// *** Constructor and helper: ***

	Menu::Menu(sf::Font font, SelectionType type, bool hasSlider, bool hideChild)
	: Component()
	, children_()
	, selectedChild_(-1)
	, hasSlider_(hasSlider)
	, hidingChildren_(hideChild)
	, nextKey_()
	, previousKey_()
{
	if(type == Vertical)
	{
		nextKey_ = sf::Keyboard::Down;
		previousKey_ = sf::Keyboard::Up;
	}
	if(type == Horizontal)
	{
		nextKey_ = sf::Keyboard::Right;
		previousKey_ = sf::Keyboard::Left;
	}


	if (hasSlider_)
	{
		Slider<int>* slider = new Slider<int>(selectedChild_,
		                                      Vector2f(50.f, 25.f),
		                                      font,
		                                      "Menu");
		slider->setOperationPlus([this](int&){selectNext();});
		slider->setOperationMinus([this](int&){selectPrevious();});

		std::shared_ptr<Component> comp (slider);

		pack(comp);
	}
}

void Menu::pack(Component::SPtr component)
{
	children_.push_back(component);
	
	if(!hasSelection() && component->isSelectable())
	{
		select(children_.size() - 1);
	}
}

//-----------------------------------------------------------------------------
// *** gui::Component behaviour: ***

bool Menu::isSelectable() const
{
	return true;
}


//-----------------------------------------------------------------------------
// *** Helper functions: ***


bool Menu::hasSelection() const
{
	return selectedChild_ >= 0;
}

void Menu::select(std::size_t index)
{
	if (children_[index]->isSelectable())
	{
		if (hasSelection())
			children_[selectedChild_]->deselect();
		
		children_[index]->select();
		selectedChild_ = index;
	}
}

void Menu::selectNext()
{
	if (!hasSelection())
		return;

	// Search next component that is selectable, wrap around if necessary
	int next = selectedChild_;
	do
		next = (next + 1) % children_.size();
	while (!children_[next]->isSelectable());

	select(next);
}

void Menu::selectPrevious()
{
	if (!hasSelection())
		return;
	
	// Search previous component that is selectable, wrap around if necessary
	int prev = selectedChild_;
	do
		prev = (prev + children_.size() - 1) % children_.size();
	while (!children_[prev]->isSelectable());

	select(prev);
}


//-----------------------------------------------------------------------------
// *** I/O: ***

void Menu::handleEvent(const sf::Event& event)
{
	/* A mouse can select everything in the screen, so the children must take
	   care of this */
	if(event.type == sf::Event::MouseButtonPressed ||
	   event.type == sf::Event::MouseButtonReleased)
	{
		for(auto& child : children_)
		{
			child->handleEvent(event);
		}
	}

	else if(event.type == sf::Event::KeyReleased)
	{
		/* An _active_ child has the exclusivity of the Event */
		if (hasSelection() && children_[selectedChild_]->isActive())
		{
			children_[selectedChild_]->handleEvent(event);
		}
		else if (hasSelection() && event.key.code == sf::Keyboard::Return)
		{
			if(children_[selectedChild_]->isActive())
			{
				children_[selectedChild_]->activate();
			}
			else
			{
				children_[selectedChild_]->deactivate();
			}
		}

		else if(hasSelection() &&
		        (!hasSlider_ || children_[0]->isSelected()))
		{
			if (event.key.code == previousKey_)
			{
				selectPrevious();
			}
			else if (event.key.code == nextKey_)
			{
				selectNext();
			}
		}
		
		else
		{
			for(auto& child : children_)
			{
				child->handleEvent(event);
			}	
		}
	}
}


void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    if(hidingChildren_ && hasSelection())
    {
	    target.draw(*children_[selectedChild_], states);
    }
    else
    {
	    for (const auto& child : children_)
	    {
		    target.draw(*child, states);
	    }
    }
}

} // namespace gui
