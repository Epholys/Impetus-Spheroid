#include "core/World.hpp"
#include "core/Target.hpp"


//-----------------------------------------------------------------------------
// *** constructor and destructor: ***

Target::Target(World* world,
               ecs::EntityManager& entm,
               Vector2f position,
               Vector2f size,
               sf::Color color)
	: Entity(world, entm, EntityID::Target)
	, rect_(size)
	, objective_()
{
	label_ = ecs::createTarget(entm, position, size);

	rect_.setFillColor(color);
	auto rectBounds = rect_.getLocalBounds();
	rect_.setOrigin(rectBounds.left + rectBounds.width / 2.f,
	                rectBounds.top + rectBounds.height / 2.f);


	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	if(pointPos)
	{
		objective_ = pointPos->position_;
	}

	update(Time());
}

Target::~Target()
{
}

//-----------------------------------------------------------------------------
// *** virtual functions: ***

void Target::update(Time dt)
{
	Entity::update(dt);

	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));

	if(pointPos)
	{
		auto position = pointPos->position_;
		setPosition(position.x, position.y);
	}

	updateObjective();
	moveToObjective();
}

void Target::draw(sf::RenderTarget& target,
                  sf::RenderStates states) const
{
	Entity::draw(target, states);

	states.transform *= getTransform();

	target.draw(rect_, states);
}


//-----------------------------------------------------------------------------
// *** special functions: ***

void Target::updateObjective()
{
	auto targetComponent = dynCast<ecs::Target>
		(ecs_.getComponent(label_, ecs::Component::Target));
		 
	if(!targetComponent) return;
	if(!targetComponent->reset()) return;


	auto positionComponent = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));

	if(!positionComponent) return;

	auto windowSize = world_->getWindowSize();
	int newXPosition = randInt(windowSize.x / 4, windowSize.x - 20);
	int newYPosition = randInt(80, windowSize.y - 80);

	objective_ = Vector2f(newXPosition, newYPosition);
}

void Target::moveToObjective()
{
	const float EPSILON = 0.1f;
	const float OLD_VEL_COEFF = 0.25f;
	const float NEW_VEL_COEFF = 2.5f;

	auto velocityComponent = dynCast<ecs::Velocity>
		(ecs_.getComponent(label_, ecs::Component::Velocity));
	auto positionComponent = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));

	if(!velocityComponent || !positionComponent) return;

	
	Vector2f vectToObjective = objective_ - positionComponent->position_;
	if(std::sqrt(std::pow(vectToObjective.x,2) + std::pow(vectToObjective.y,2)) < EPSILON)
	{
		velocityComponent->velocity_ = Vector2f();
	}
	else
	{
		velocityComponent->velocity_ =
			velocityComponent->velocity_ * OLD_VEL_COEFF +
			vectToObjective * NEW_VEL_COEFF;
	}
}
