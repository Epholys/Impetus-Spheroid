#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP


#include <deque>

#include <SFML/Graphics.hpp>

#include "data/ParticleData.hpp"

class ParticleSystem : public sf::Drawable
{
public:
	ParticleSystem(Particle::Type type);

	void addParticle (Vector2f position, Vector2f velocity);
	void addParticle (Vector2f position, Vector2f velocity, sf::Color color);
	void addParticle (Vector2f position, Vector2f velocity, sf::Color color, float angle, float scale);

	void update (Time dt);
	void draw (sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void addVertex (float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;
	void computeVertices() const;
	
private:
	std::deque<Particle> particles_;
	std::vector<ParticleAffector> affectors_;
	Particle::Type type_;
	sf::Texture texture_;

	mutable sf::VertexArray vertexArray_;
	mutable bool updateVertices_;
};


#endif // PARTICLE_SYSTEM_HPP
