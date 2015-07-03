#ifndef SUN_HIGHLIGHT_HPP
#define SUN_HIGHLIGHT_HPP


#include <algorithm>
#include <limits>

#include <SFML/Graphics.hpp>

#include "utility/Vector2.hpp"
#include "utility/Time.hpp"

class SunHighlight : public sf::Drawable   
{
public:
	SunHighlight(Vector2f position, sf::FloatRect borders);

	void update(Time dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Vector2f polarToCartesian(Vector2f polarCoord);
	float distance(Vector2f u, Vector2f v);
	
private:
	Vector2f position_;

	float borderRadius_;

	float baseAngle_;
	float angularVelocity_;
	int nSubdivision_;

	sf::Color color_;
	sf::VertexArray view_;
};



#endif // SUN_HIGHLIGHT_HPP
