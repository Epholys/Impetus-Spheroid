#include "gui/TextureGenerator.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto datas = genMarketData();
}

namespace gui 
{
	namespace TextureGenerator
	{
		std::unique_ptr<sf::Texture> marketButton(unsigned int width,
		                                          unsigned int height,
		                                          const std::string& icon,
		                                          const std::string& fontPath,
		                                          int number,
		                                          const std::string& legend,
		                                          int price,
		                                          const std::string& shortcut)
		{
			sf::RenderTexture rTexture;
			
			assert(rTexture.create(width, height));

			sf::Font font;
			font.loadFromFile(fontPath);

			std::stringstream ss;

			sf::Texture iconTexture;
			iconTexture.loadFromFile(icon);
			auto iconSize = iconTexture.getSize();
			sf::Sprite iconSprite(iconTexture);
			float scale = float(height) / iconSize.y;
			iconSprite.setScale(scale, scale);
			 
			ss.str("");
			ss << "x" << number;
			sf::Text numberItems(ss.str(), font);
			numberItems.setColor(sf::Color::Green);
			int chSize = height * 0.7f;
			numberItems.setCharacterSize(chSize);
			centerOrigin(numberItems);
			numberItems.setPosition(iconSize.x * scale * 1.5f,
			                        (height - chSize) / 2.f);
			
			sf::Text legendText(legend, font);
			legendText.setColor(sf::Color::Black);
			chSize = height * 0.4f;
			legendText.setCharacterSize(chSize);
			centerOrigin(legendText);
			legendText.setPosition(width / 2.f, (height - chSize) / 2.f );
			
			ss.str("");
			ss << price;
			sf::Text priceText(ss.str(), font);
			priceText.setColor(sf::Color::Yellow);
			chSize = height * 0.7f;
			priceText.setCharacterSize(chSize);
			centerOrigin(priceText);
			priceText.setPosition(width * 0.9f, (height - chSize) / 2.f);

			rTexture.clear(sf::Color(230,230,230));
			rTexture.draw(iconSprite);
			rTexture.draw(numberItems);
			rTexture.draw(legendText);
			rTexture.draw(priceText);
			rTexture.display();

			sf::RenderTexture rTextureShortcut;
			
			assert(rTextureShortcut.create(width + height, height));

			sf::Sprite firstPart(rTexture.getTexture());
				
			sf::Text shortcutText(shortcut, font);
			centerOrigin(shortcutText);
			shortcutText.setPosition(width + height / 2.f, (height - shortcutText.getCharacterSize()) / 2.f);
			
			rTextureShortcut.clear(sf::Color::Black);
			rTextureShortcut.draw(firstPart);
			rTextureShortcut.draw(shortcutText);
			rTextureShortcut.display();
			
			std::unique_ptr<sf::Texture> textPtr (new sf::Texture(rTextureShortcut.getTexture()));
			return std::move(textPtr);
		}

		void createMarketButtons(const std::map<PowerUpID::ID, sf::Keyboard::Key>& KEYS)
		{
			const sf::Vector2u BUTTON_SIZE (600, 50);
			for(auto it=datas.begin(); it!=datas.end(); ++it)
			{
				(*(gui::TextureGenerator::marketButton(
					BUTTON_SIZE.x,
					BUTTON_SIZE.y,
					it->texturePath,
					"./media/font/FORCEDSQUARE.ttf",
					it->number,
					it->description,
					it->price,
					toString(KEYS.at(it->id)))))
					.copyToImage()
					.saveToFile(it->texturePath + ".button.png");
			}
		}
	}
}
