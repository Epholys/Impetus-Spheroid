#ifndef GUI_COMPONENT_HPP
#define GUI_COMPONENT_HPP

#include <memory>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace gui
{
	/* Abstract class implementing or defining some basic logic shared by all
	 * gui elements:
	 *   - sf::Drawable
	 *   - sf::Transformable
	 *   - Event handling
	 *   - Selection
	 *   - Activation
	 * */


class Component : public sf::Drawable, public sf::Transformable
{
public:
	typedef std::shared_ptr<Component> SPtr;

public:
	Component();
	virtual ~Component();

	/* A gui::Component can be selectable, like a button, or not, like a
	   container. */
	virtual bool isSelectable() const =0;
	bool isSelected() const;
	virtual void select();
	virtual void deselect();

	bool isActive() const;
	virtual void activate();
	virtual void deactivate();

	virtual void handleEvent(const sf::Event& event) =0;

	void setParent(Component* comp);
	sf::Transform getGlobalTransform() const;

protected:
    Component* parent_;
	
private:
	bool isSelected_;
	bool isActive_;
};

} // namespace gui


#endif // GUI_COMPONENT_HPP
