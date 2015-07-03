#include "core/SunHighlight.hpp"


//-----------------------------------------------------------------------------

namespace
{
	const float PI = 3.1415926535f;

	const float ANGULAR_VELOCITY = PI / 8.f;
	const int N_SUBDIVISION = 48;

	const sf::Color COLOR = sf::Color(255, 230, 0, 15);
}


//-----------------------------------------------------------------------------

SunHighlight::SunHighlight(Vector2f position, sf::FloatRect borders)
	: position_(position)
	, borderRadius_(std::numeric_limits<float>::max())
	, baseAngle_(0.f)
	, angularVelocity_(ANGULAR_VELOCITY)
	, nSubdivision_(N_SUBDIVISION)
	, color_(COLOR)
	, view_(sf::Triangles)
{
	update(Time());

	std::vector<Vector2f> edges{ {borders.left, borders.top},
		                         {borders.left + borders.width, borders.top},
		                         {borders.left, borders.top + borders.height},
		                         {borders.left + borders.width, borders.top + borders.height} };
}


//-----------------------------------------------------------------------------

void SunHighlight::update(Time dt)
{
	baseAngle_ += angularVelocity_ * dt.asSeconds();

	view_.clear();
	float zoneAngle = 2 * PI / nSubdivision_;
	for (int i=0; i<nSubdivision_ - 1 ; ++i)
	{
		sf::Color color = !(i % 2) ? color_ : sf::Color(0,0,0,0);
		view_.append(sf::Vertex(position_, color));
		view_.append(sf::Vertex(polarToCartesian(Vector2f(borderRadius_,
		                                                  baseAngle_ + (zoneAngle * i)))
		                        + position_,
		                        color));
		view_.append(sf::Vertex(polarToCartesian(Vector2f(borderRadius_,
		                                                  baseAngle_ + zoneAngle*(i+1)))
		                        + position_,
		                        color));
	}
}

Vector2f SunHighlight::polarToCartesian(Vector2f polarCoord)
{
	return Vector2f(polarCoord.x * std::cos(polarCoord.y),
	                polarCoord.x * std::sin(polarCoord.y));
}

float SunHighlight::distance(Vector2f u, Vector2f v)
{
	return std::sqrt(std::pow(v.x - u.x, 2) + std::pow(v.y - u.y, 2));
}


//-----------------------------------------------------------------------------

void SunHighlight::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(view_, states);
}
