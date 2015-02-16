#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <vector>

#include <SFML/Window/Event.hpp>

#include "gui/Component.hpp"
#include "gui/Slider.hpp"

namespace gui
{

class Menu : public Component
{
public:
	enum SelectionType
	{
		None = 0,
		Vertical,
		Horizontal,
	};

public:
	typedef std::shared_ptr<Menu> SPtr;

public:
	Menu(SelectionType type, bool hasSlider = false, bool hideChild = false);

	void pack(Component::SPtr component);

	virtual bool isSelectable() const;
	virtual void handleEvent(const sf::Event& event);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	bool hasSelection() const;
	void select(std::size_t index);
	void selectNext();
	void selectPrevious();
	
protected:
	std::vector<Component::SPtr> children_;
	int selectedChild_;

	bool hasSlider_;
	bool hidingChildren_;

	sf::Keyboard::Key nextKey_;
	sf::Keyboard::Key previousKey_;
};


}

#endif // GUI_MENU_HPP
