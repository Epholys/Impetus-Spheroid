#ifndef GUI_MENU_META_HPP
#define GUI_MENU_META_HPP


#include <vector>

#include <SFML/Graphics.hpp>

#include "framework/Assertion.hpp"
#include "utility/utility.hpp"
#include "gui/Menu.hpp"
#include "gui/Button.hpp"

namespace gui
{
	class MenuMeta : public Menu
	{
	public:
		typedef std::shared_ptr<MenuMeta> SPtr;
		
	public:
		MenuMeta(Menu::SelectionType type);
		virtual ~MenuMeta();

		void pack(Menu::SPtr menu, const std::string& name);

		virtual void handleEvent(const sf::Event& event);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		virtual void select();
		
	protected:
		virtual void select(std::size_t index);
		
		void addButton(const std::string& name);
		void createButtonTexture(sf::Texture& texture, Vector2f size,  const std::string& name);

	protected:
		sf::Font font_;

		struct ButtonEntry
		{
			gui::Button button;
			std::string name;
		};
		std::vector<ButtonEntry> buttons_;
	};
}


#endif // GUI_MENU_META_HPP
