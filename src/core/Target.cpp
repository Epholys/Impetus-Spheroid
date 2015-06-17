#include "core/Target.hpp"
#include "core/World.hpp"
#include "core/PowerUpFunctions.hpp"

//-----------------------------------------------------------------------------

namespace
{
	const Vector2f SIZE {15.f, 75.f};
	const sf::Color COLOR = sf::Color::Yellow;
}


//-----------------------------------------------------------------------------
// *** constructor and destructor: ***

Target::Target(World& world,
               Vector2f position)
	: Entity(world, world.getEntityManager(), EntityType::Target)
	, rect_(SIZE)
	, objective_()
{
	label_ = ecs::createTarget(ecs_, position, SIZE);

	centerOrigin(rect_);
	rect_.setFillColor(COLOR);

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
		
		// NOT rect_.setPosition(...): rect_'s Transformable base class isn't
		// used because if suddenly Target's view become several sf::Sprites and
		// sf::Shapes, it would mean I'll have to update every single one of them.
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

void Target::updateColor()
{
	const float EPSILON = 0.01f;
	const float MULTIPLIER = PowerUpDatas::TARGET_POINT_MULTIPLIER;	
	const std::vector<sf::Color> COLORS
			{sf::Color::Yellow,
			sf::Color::Green,
			sf::Color(0,255,255),
			sf::Color::Blue,
			sf::Color(125,0,255),
			sf::Color::Red,
			sf::Color(255,175,0)};

	auto targetComp = dynCast<ecs::Target>
		(ecs_.getComponent(label_, ecs::Component::Target));
	if(targetComp)
	{
		auto mult = targetComp->getPointMultiplier();
		// 50 ios an arbitrary big value, I suppose nobody will have enough
		// patience to cumulate so much money.
		for(int i=0; i<50; ++i)
		{
			if(std::abs(mult - std::pow(MULTIPLIER, i)) < EPSILON)
			{
				rect_.setFillColor(COLORS[((i)%(COLORS.size()))]);
				break;
			}
		}
	}
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

	const int Y_MARGIN = 80;
	const int X_RIGHT_MARGIN = 20;
	auto windowSize = world_.getWindowSize();
	int newXPosition = randInt(windowSize.x / 4, windowSize.x - X_RIGHT_MARGIN);
	int newYPosition = randInt(Y_MARGIN, windowSize.y - Y_MARGIN);

	objective_ = Vector2f(newXPosition, newYPosition);
}

void Target::moveToObjective()
{
	// Note : I could simplify a little this function by removing averything
	// about OLD_VEL_COEFF, but then I'll have to find again the sweet spot of
	// NEW_VEL_COEFF, and I like the speed as it is now.
	const float EPSILON = 0.1f;
	const float OLD_VEL_COEFF = 0.25f;
	const float NEW_VEL_COEFF = 2.5f;

	auto velocityComponent = dynCast<ecs::Velocity>
		(ecs_.getComponent(label_, ecs::Component::Velocity));
	auto positionComponent = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));

	if(!velocityComponent || !positionComponent) return;


	Vector2f vectToObjective = objective_ - positionComponent->position_;
	float distanceToObjective = std::sqrt(std::pow(vectToObjective.x,2) + std::pow(vectToObjective.y,2));
	if(distanceToObjective < EPSILON)
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
