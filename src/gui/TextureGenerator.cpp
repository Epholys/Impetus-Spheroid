#include "gui/TextureGenerator.hpp"


//-----------------------------------------------------------------------------

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
		                                          int price)
		{
			sf::RenderTexture rTexture;
			
			assert(rTexture.create(width, height));

			sf::Font font;
			font.loadFromFile(fontPath);

			std::stringstream ss;

			sf::Texture iconTexture;
			iconTexture.loadFromFile(icon);
			iconTexture.setSmooth(true);
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

			std::unique_ptr<sf::Texture> textPtr (new sf::Texture(rTexture.getTexture()));
			return std::move(textPtr);
		}
	}
}
