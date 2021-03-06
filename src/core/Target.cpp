#include <sstream>
#include "framework/Assertion.hpp"
#include "core/Target.hpp"
#include "core/World.hpp"
#include "core/PowerUpFunctions.hpp"
#include "data/BallData.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------
// *** constructor and destructor: ***

Target::Target(World& world,
               Vector2f position)
	: Entity(world, world.getEntityManager(), EntityType::Target)
	, shape_(ecs::Collidable::Rectangle)
	, rect_(TargetDefault::SIZE)
	, circ_(TargetDefault::RADIUS)
	, position_(nullptr)
	, font_(world_.getFontRef())
	, indicators_()
	, objective_()
{
	label_ = ecs::createTarget(ecs_, position, TargetDefault::SIZE);

	centerOrigin(rect_);
	rect_.setFillColor(TargetDefault::COLOR);

	centerOrigin(circ_);
	circ_.setFillColor(TargetDefault::COLOR);

	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	if(pointPos)
	{
		position_ = &pointPos->position_;
		objective_ = pointPos->position_;
	}
	
	update(Time());
}

Target::~Target()
{
}

//-----------------------------------------------------------------------------

Vector2f Target::getSize() const
{
	return
		(shape_ == ecs::Collidable::Rectangle) ?
			Vector2f(rect_.getSize()) :
			Vector2f(circ_.getRadius(), circ_.getRadius());
}


//-----------------------------------------------------------------------------
// *** virtual functions: ***

void Target::update(Time dt)
{
	Entity::update(dt);

	if(position_)
	{
		// NOT rect_.setPosition(...): rect_'s Transformable base class isn't
		// used because if suddenly Target's view become several sf::Sprites and
		// sf::Shapes, it would mean I'll have to update every single one of them.
		setPosition(position_->x, position_->y);
	}
	
	updateIndicators(dt);
	updateObjective();
	moveToObjective();
}

void Target::draw(sf::RenderTarget& target,
                  sf::RenderStates states) const
{
	Entity::draw(target, states);

	for(const auto entry : indicators_)
	{
		target.draw(entry.text, states);
	}

	states.transform *= getTransform();


	if(shape_ == ecs::Collidable::Rectangle)
		target.draw(rect_, states);
	else
		target.draw(circ_, states);
}


//-----------------------------------------------------------------------------

void Target::updateColor()
{
	const float EPSILON = 0.01f;
	const float MULTIPLIER = PowerUpDatas::TARGET_POINT_MULTIPLIER;	
	const std::vector<sf::Color> COLORS
			{TargetDefault::COLOR,
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
		// 50 is an arbitrary big value, I suppose nobody will have enough
		// patience to cumulate so much money.
		for(int i=0; i<50; ++i)
		{
			if(std::abs(mult - std::pow(MULTIPLIER, i)) < EPSILON)
			{
				rect_.setFillColor(COLORS[((i)%(COLORS.size()))]);
				circ_.setFillColor(COLORS[((i)%(COLORS.size()))]);
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

	if(!position_) return;

	const int Y_MARGIN = 80;
	int xSize = (shape_ == ecs::Collidable::Rectangle) ? rect_.getSize().x : circ_.getRadius();
	const int X_MARGIN = 80.f + xSize;
	auto windowSize = world_.getWindowSize();
	int newXPosition = randInt(X_MARGIN, windowSize.x - X_MARGIN);
	int newYPosition = randInt(Y_MARGIN, windowSize.y - Y_MARGIN);

	objective_ = Vector2f(newXPosition, newYPosition);
}

void Target::updateIndicators(Time dt)
{
	const Vector2f START (getPosition().x -20.f, getPosition().y - TargetDefault::SIZE.y - 5.f);
	const Vector2f FINISH (START.x, START.y - 50.f);
	const Time DURATION = seconds(1.f);
	
	const auto& collisions = world_.getTrackedCollisions();
	for(const auto& pair : collisions)
	{
		if(pair.second == getLabel())
		{
			auto projectileComp =
				dynCast<ecs::Projectile>(ecs_.getComponent(pair.first,
				                                           ecs::Component::Projectile));
			auto targetComp =
				dynCast<ecs::Target>(ecs_.getComponent(pair.second,
				                                       ecs::Component::Target));
			assert(projectileComp && targetComp);
			float pointsWon = projectileComp->getPoints() * targetComp->getPointMultiplier();
			std::size_t index = findIndex(pointsWon);
			const float BASE_SCALE = 0.5f, MAX_SCALE = 2.f;
			float scale = BASE_SCALE * (1 + index * (MAX_SCALE - BASE_SCALE) / ballDatas.size());
			
			std::stringstream ss;
			ss << "+" << static_cast<int>(pointsWon);
			sf::Text text (ss.str(), font_);
			text.setColor(ballDatas[index].color);
			gui::Transition transition (nullptr,
			                            gui::Transition::Quadratic,
			                            gui::TransformData(START, 0.f, Vector2f(scale, scale)),
			                            gui::TransformData(FINISH, 0.f, Vector2f(scale, scale)),
			                            DURATION);
			gui::FadeOut<sf::Text> fadeOut {nullptr, DURATION * 1.5f, Time()};
			indicators_.push_front({text, transition, fadeOut});
			indicators_.front().transition.setTransformable(&indicators_.front().text);
			indicators_.front().fadeOut.pT_ = &indicators_.front().text;
		}
	}

	for(auto& entry : indicators_)
	{
		entry.transition.update(dt);
		entry.fadeOut.update(dt);
	}
	while(!indicators_.empty()
	      && indicators_.back().transition.isOver())
	{
		indicators_.pop_back();
	}
}


std::size_t Target::findIndex(int points)
{
	std::size_t index = 0;
	for(; index < ballDatas.size(); ++index)
	{
		if(ballDatas[index].point > points)
		{
			break;
		}
	}
	index = (index) ? index-1 : index;
	return index;
}



void Target::moveToObjective()
{
	// Note : I could simplify a little this function by removing averything
	// about OLD_VEL_COEFF, but then I'll have to find again the sweet spot of
	// NEW_VEL_COEFF, and I like the speed as it is now.

	// Second Note: I could change this by using MultiTransition
	const float EPSILON = 0.1f;
	const float OLD_VEL_COEFF = 0.25f;
	const float NEW_VEL_COEFF = 2.5f;

	auto velocityComponent = dynCast<ecs::Velocity>
		(ecs_.getComponent(label_, ecs::Component::Velocity));

	if(!velocityComponent || !position_) return;


	Vector2f vectToObjective = objective_ - *position_;
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


//-----------------------------------------------------------------------------

void Target::changeSize(Vector2f newSize)
{
	bool force = true;
	auto collidableComponent = dynCast<ecs::Collidable>(ecs_.getComponent(label_, ecs::Component::Collidable, force));
	assert(collidableComponent);

	rect_.setSize(newSize);
	centerOrigin(rect_);

	float radius = std::sqrt(newSize.x * newSize.x + newSize.y * newSize.y) / 2.f;
	circ_.setRadius(radius);
	centerOrigin(circ_);

	if(shape_ == ecs::Collidable::Rectangle)
	{
		collidableComponent->size = Vector2f(rect_.getSize());
	}
	else
	{
		collidableComponent->radius = circ_.getRadius();
	}
}

void Target::setShape(ecs::Collidable::Type shape)
{
	shape_ = shape;

	bool force = true;
	auto collidableComponent = dynCast<ecs::Collidable>(ecs_.getComponent(label_, ecs::Component::Collidable, force));
	assert(collidableComponent);
	collidableComponent->type = shape;

	if(shape_ == ecs::Collidable::Rectangle)
	{
		collidableComponent->size = Vector2f(rect_.getSize());
	}
	else
	{
		collidableComponent->radius = circ_.getRadius();
	}	
}
