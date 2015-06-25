#include "core/ParticleSystem.hpp"


//-----------------------------------------------------------------------------
// *** datas: ***

namespace
{
	std::vector<ParticleData> datas = genParticleData();
	std::vector<ParticleAffector> affectors = genParticleAffectors();
}


//-----------------------------------------------------------------------------

ParticleSystem::ParticleSystem(Particle::Type type)
	: particles_()
	, affectors_()
	, type_(type)
	, texture_()
	, vertexArray_(sf::Quads)
	, updateVertices_(true)
{
	for(auto& affectorType : datas.at(type).affectors)
	{
		affectors_.push_back(affectors[affectorType]);
	}
	
	texture_.loadFromFile(datas.at(type).texturePath);
}


//-----------------------------------------------------------------------------

void ParticleSystem::addParticle(Vector2f position, Vector2f velocity)
{
	Particle particle;
	particle.position = position;
	particle.velocity = velocity;
	particle.color = datas.at(type_).color;
	particle.lifetime = datas.at(type_).lifetime;

	particles_.push_back(particle);
}

void ParticleSystem::addParticle(Vector2f position, Vector2f velocity, sf::Color color)
{
	addParticle(position, velocity);
	particles_.back().color = color;
}


//-----------------------------------------------------------------------------

void ParticleSystem::update(Time dt)
{
	while(!particles_.empty() && particles_.front().lifetime <= sf::Time::Zero)
	{
		particles_.pop_front();
	}

	for (auto& particle : particles_)
	{
		particle.lifetime -= dt;
		particle.position += particle.velocity * dt.asSeconds();

		for(auto& affector : affectors_)
		{
			affector(particle, type_, dt);
		}
	}


	updateVertices_ = true;
}


//-----------------------------------------------------------------------------

void ParticleSystem::draw (sf::RenderTarget& target, sf::RenderStates states) const
{
	if(updateVertices_)
	{
		computeVertices();
		updateVertices_ = false;
	}

	states.texture = &texture_;

	target.draw(vertexArray_, states);
}

void ParticleSystem::computeVertices() const
{
	Vector2f textureSize (texture_.getSize());
	Vector2f half = textureSize / 2.f;

	vertexArray_.clear();

	for(const auto& particle : particles_)
	{
		Vector2f position = particle.position;
		sf::Color color = particle.color;

		addVertex(position.x - half.x, position.y - half.y, 0.f,           0.f,           color);
		addVertex(position.x + half.x, position.y - half.y, textureSize.x, 0.f,           color);
		addVertex(position.x + half.x, position.y + half.y, textureSize.x, textureSize.y, color);
		addVertex(position.x - half.x, position.y + half.y, 0.f,           textureSize.y, color);
	}
}

void ParticleSystem::addVertex (float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
	Vector2f xyCoord (worldX, worldY);
	Vector2f uvCoord (texCoordX, texCoordY);

	vertexArray_.append(sf::Vertex(xyCoord, color, uvCoord));
}
