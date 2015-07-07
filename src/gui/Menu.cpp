#include "gui/Menu.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace gui
{

//-----------------------------------------------------------------------------
// *** Constructor and helper: ***

	Menu::Menu(SelectionType type)
	: Component()
	, children_()
	, selectedChild_(-1)
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


	// if (hasSlider)
	// {
	// 	std::shared_ptr<Slider<int>> slider (new Slider<int>(selectedChild_,
	// 	                                                     Vector2f(50.f, 25.f),
	// 	                                                     name));
	// 	slider->setOperationPlus([](int&){});
	// 	slider->setOperationMinus([](int&){});


	// 	menuSlider_ = slider.get();
	// 	pack(std::move(slider));
	// }
}

void Menu::pack(Component::SPtr component)
{
	// if(overrideFirst)
	// {
	// 	auto it = children_.begin();
	// 	children_.insert(it, component);
	// }
	// else
	// {
		children_.push_back(component);
	// }

	// auto childMenu = std::dynamic_pointer_cast<Menu>(component);
	// if(childMenu && isMeta_)
	// {
	// 	childMenu->pack(children_[0], true);
	// 	childMenu->menuSlider_ = menuSlider_;
	// }
}

Menu::~Menu()
{
}

//-----------------------------------------------------------------------------
// *** gui::Component behaviour: ***
bool Menu::isSelectable() const
{
	return true;
}


void Menu::select()
{
	Component::select();

	if(children_.empty())
		return;

	if(!hasSelection())
	{
		std::size_t index = 0;
		while(!children_[index]->isSelectable() && index < children_.size())
		{
			++index;
		} 

		if(index != children_.size())
		{
			select(index);
		}

		// if(menuSlider_)
		// {
		// 	menuSlider_->updateText();
		// 	menuSlider_->setName(name_);
		// }
	}
}

void Menu::deselect()
{
	Component::deselect();
	selectedChild_ = -1;
	// if(menuSlider_)
	// {
	// 	menuSlider_->setName("Main Menu");
	// }
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
	   event.type == sf::Event::MouseButtonReleased ||
	   event.type == sf::Event::MouseMoved)
	{
		for(auto& child : children_)
		{
			child->handleEvent(event);
		}
	}

	else if(event.type == sf::Event::KeyReleased)
	{
		if(!hasSelection() || !isSelected())
			return;

		if (event.key.code == sf::Keyboard::Return &&
		         !children_[selectedChild_]->isActive())
		{
			children_[selectedChild_]->activate();
		}
		else if (event.key.code == sf::Keyboard::Return &&
		         children_[selectedChild_]->isActive())
		{
			children_[selectedChild_]->deactivate();
		}

		else if (children_[selectedChild_]->isActive())
		{
			children_[selectedChild_]->handleEvent(event);
		}

		else if( /*(!isMeta_ || children_[0]->isSelected()) &&*/
		         event.key.code == previousKey_)
		{
			selectPrevious();
		}
		else if( /*(!isMeta_ || children_[0]->isSelected()) &&*/
		         event.key.code == nextKey_)
		{
			selectNext();
		}

		
		else
		{
			for(auto& child : children_)
			{
				child->handleEvent(event);
			}
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


void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    // if(hidingChildren_ && hasSelection())
    // {
	//     bool menuSliderDrawn = false;

	//     if(menuSlider_ && !std::dynamic_pointer_cast<Menu>(children_[selectedChild_]))
	//     {
	// 	    target.draw(*menuSlider_, states);
	// 	    menuSliderDrawn = true;
	//     }

	//     if(!menuSliderDrawn)
	//     {
	// 	    target.draw(*children_[selectedChild_], states);
	//     }
    // }
    // else
    // {
	    for (const auto& child : children_)
	    {
		    target.draw(*child, states);
	    }
    // }
}

} // namespace gui
