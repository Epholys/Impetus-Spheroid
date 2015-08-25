#include "utility/random.hpp"
#include "core/Ball.hpp"
#include "core/World.hpp"

//-----------------------------------------------------------------------------

namespace
{
	const float RADIUS = 10.f;
	const float MASS = 1.f;
	const unsigned int PARTICLE_RATE = 300;
}


//-----------------------------------------------------------------------------

const float Ball::RADIUS_ = RADIUS;
const float Ball::MASS_ = MASS;


//-----------------------------------------------------------------------------

Ball::Ball(World& world,
           Vector2f position, 
           Vector2f gravVect,
           BallData data,
           int nCollisionTarget,
           unsigned int type)
	: Entity(world, world.getEntityManager(), EntityType::Ball)
	, ball_(RADIUS)
	, position_(nullptr)
	, trailEmitter_(-1)
{
	label_ = ecs::createBall(ecs_, position, RADIUS, MASS, gravVect, data.point, nCollisionTarget);


	auto positionComponent = dynCast<ecs::Position>
	                                  (ecs_.getComponent(label_, ecs::Component::Position));
	assert(positionComponent);
	position_ = &positionComponent->position_;

	
	sf::Color ballColor = data.color;
	
	if(type & Massless)
	{
		ecs_.removeComponent(label_, ecs::Component::Mass);
	}
	if(type & Ghost)
	{
		ecs_.removeComponent(label_, ecs::Component::Solid);
		ballColor.a = 100;
	}

	trailEmitter_ = addParticleEmitter(Particle::BallTrail, *position_, PARTICLE_RATE, ballColor);
	
	centerOrigin(ball_);
	ball_.setFillColor(ballColor);
	ball_.setOutlineThickness(-3.f);
	ball_.setOutlineColor(ballColor);

	update(Time());
}

Ball::~Ball()
{
}


//-----------------------------------------------------------------------------
// *** Virtual functions: ***


void Ball::update(Time dt)
{
	// NOT ball_.setPosition(...): ball_'s Transformable base class isn't
	// used because if suddenly Ball's view become several sf::Sprites and
	// sf::Shapes, it would mean I'll have to update every single one of them.
	setPosition(position_->x, position_->y);

	if(trailEmitter_ != -1)
	{
		updatePosition(trailEmitter_, *position_);
	}

	Entity::update(dt);

	
	if(trailEmitter_ == -1) return;
	
	auto projectileComponent = dynCast<ecs::Projectile>
		(ecs_.getComponent(label_, ecs::Component::Projectile));
	if(projectileComponent && projectileComponent->hasTouchedTarget())
	{
		removeParticleEmitter(trailEmitter_);
		trailEmitter_ = -1;

		makeMicroParticles(dt, projectileComponent);
	}
}

void Ball::makeMicroParticles(Time dt, std::shared_ptr<ecs::Projectile> projectileComponent)
{
	std::vector<int> microParticleEmitterID;
	unsigned int rate = std::ceil(1.f / dt.asSeconds());

	auto velocityComponent = dynCast<ecs::Velocity>
		(ecs_.getComponent(label_, ecs::Component::Velocity));
	assert(velocityComponent);
	Vector2f velocity = velocityComponent->velocity_;

	for(int i = 0; i<projectileComponent->getPoints() * 3; ++i)
	{
		Vector2f speed (normalRandFloat(velocity.x / 3.f, velocity.x / 3.f),
		                normalRandFloat(velocity.y / 3.f , velocity.y / 3.f));
		
		microParticleEmitterID.push_back(
			addParticleEmitter(Particle::Shard, *position_, rate, ball_.getFillColor(), speed));
	}
	Emitter::update(dt);
	for(int i : microParticleEmitterID)
	{
		removeParticleEmitter(i);
	}
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	
	states.transform *= getTransform();

	target.draw(ball_, states);
}
