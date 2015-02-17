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
	virtual ~Menu();

	void pack(Component::SPtr component, bool overrideFirst = false);

	virtual bool isSelectable() const;
	virtual void handleEvent(const sf::Event& event);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void select();
	virtual void deselect();

protected:
	bool hasSelection() const;
	void select(std::size_t index);
	void selectNext();
	void selectPrevious();
	
protected:
	std::vector<Component::SPtr> children_;
	int selectedChild_;
	Component* menuSlider_;

	bool hidingChildren_;
	bool isMeta_;

	sf::Keyboard::Key nextKey_;
	sf::Keyboard::Key previousKey_;
};


}

#endif // GUI_MENU_HPP
