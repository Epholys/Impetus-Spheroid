#include "core/World.hpp"
#include "core/MetaData.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------
// *** constructor: ***

//const Time World::TIME_BEETWEEN_FIRE = milliseconds(333);
Vector2f World::CANON_POSITION {40.f, 580.f};


World::World(const Vector2u& originalSize,
             MetaData& metaData,
             int precision)
	: originalSize_(originalSize)
	, ecs_()
	, physEng_(ecs_, precision)
	, evtGen_()
	, difficulty_(DifficultyContext{this, &evtGen_, &metaData, &(metaData.inventory)})
	, inventory_(metaData.inventory)
	, cannon_(CANON_POSITION, *this, inventory_)
	, cannonModifiers_()
	, state_(Waiting)
	, speedCoeff_(1.f)
	, entities_()
	, entitiesModifiers_()
	, gravityVect_(0.f, 1000.f)
{
	const Vector2f SCORE_POSITION (750.f, 0.f);

	generateWorld();
}

void World::generateWorld()
{
	Entity::Ptr ceiling (new Wall(*this,
	                              Vector2f(originalSize_.x / 2.f, 0.f),
	                              Vector2f(originalSize_.x, 10.f),
	                              sf::Color(80,80,80)));
	
	Entity::Ptr leftWall (new Wall(*this,
	                               Vector2f(12.5f, originalSize_.y / 2.f),
	                               Vector2f(25.f, originalSize_.y),
	                               sf::Color(80,80,80)));

	Entity::Ptr rightWall (new Wall(*this,
	                                Vector2f(originalSize_.x, originalSize_.y / 2.f),
	                                Vector2f(10.f, originalSize_.y),
	                                sf::Color(80,80,80)));

	entities_.push_back(std::move(ceiling));
	entities_.push_back(std::move(leftWall));
	entities_.push_back(std::move(rightWall));

	createTarget(Vector2f(3* originalSize_.x / 4.f, originalSize_.y / 2.f));
}


ecs::EntityManager& World::getEntityManager()
{
	return ecs_;
}

LastGameData World::getGameStats() const
{
	LastGameData data;
	data.lastObjective = difficulty_.getObjective();

	return data;
}

Vector2f& World::getGravityVect()
{
	return gravityVect_;
}
Vector2u World::getWindowSize() const
{
	return Vector2u(originalSize_);
}
Vector2f World::getMousePosition() const
{
	return mousePosition_;
}

void World::addEntityModifier(Modifier<Entity> modifier)
{
	entitiesModifiers_.push_back(modifier);
}

void World::addCannonModifier(Modifier<Cannon> modifier)
{
	cannonModifiers_.push_back(modifier);
}

void World::addEntity(Entity::Ptr entity)
{
	entities_.push_back(std::move(entity));
}

void World::removeEntity(ecs::Entity label)
{
	ecs_.removeEntity(label);
	
	entities_.erase(
		std::remove_if(entities_.begin(),
		               entities_.end(),
		               [label](const Entity::Ptr& pe)
		               {
			               return pe->getLabel() == label;
		               }),
		entities_.end());
}

bool World::isGameOver() const
{
	return state_ == GameOver;
}

void World::setState(GameState state)
{
	state_ = state;
}

void World::cancelEvents(bool comeFromInventory)
{
	Modifiable<World>::forceEndingModifiers(*this);
	if(comeFromInventory)
	{
		inventory_.decrement(PowerUpID::CancelEvents);
	}
}

void World::addTime(Time adding)
{
	difficulty_.addTime(adding);
}

void World::updateDifficulty(DifficultyWorld diff)
{
	speedCoeff_ += diff.speedConstant;
}

const std::vector<eg::PhysicEngine::entityPair>&
World::getTrackedCollisions() const
{
	return physEng_.getTrackedCollisions();
}

//-----------------------------------------------------------------------------
// *** gameloop functions: ***

ecs::Entity World::createTarget(Vector2f position)
{
	Entity::Ptr pTarget (new Target(*this, position));

	ecs::Entity label = pTarget->getLabel();
	entities_.push_back(std::move(pTarget));

	return label;
}

void World::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::MouseMoved)
	{
		mousePosition_ = Vector2f(event.mouseMove.x, event.mouseMove.y);
	}

	else if(state_ == Waiting || state_ == GameOver)
	{
		if(event.type == sf::Event::MouseButtonPressed)
			state_ = Playing;
		else
			return;
	}

	cannon_.handleInput(event);
}

void World::update(Time dt)
 {
	if(state_ == Waiting)
		return;

	// All the objects that requires the "real" time:
	if(state_ != GameOver)
	{
		difficulty_.update(dt);
	}

	getEvent(dt);

	// All the objects that requires the faster time below:
	dt *= speedCoeff_;

	if(!(state_ == Waiting || state_ == GameOver))
		cannon_.update(dt);

	ecs_.update(dt);
	applyModifiers(dt);

	for(const auto& ent : entities_)
	{
		ent->update(dt);
	}
	physEng_.update(dt);

	cleanEntities();
	cleanModifiers();
}


void World::getEvent(Time dt)
{
	evt::Event evt = evtGen_.update(dt);

	if(evt.diff == evt::Event::None)
		return;
	
	if(!evt.entityModifiers.empty())
	{
		entitiesModifiers_.insert(entitiesModifiers_.begin(),
		                          evt.entityModifiers.begin(),
		                          evt.entityModifiers.end());
	}
	if(!evt.worldModifiers.empty())
	{
		modifiers_.insert(modifiers_.begin(),
		                  evt.worldModifiers.begin(),
		                  evt.worldModifiers.end());
	}
}

void World::cleanModifiers()
{
	Modifiable<World>::cleanModifiers();

	entitiesModifiers_.clear();
	cannonModifiers_.clear();
}

void World::applyModifiers(Time dt)
{
	Modifiable<World>::applyModifiers(*this, dt);

	for(auto& entity : entities_)
	{
		for(auto& modifier : entitiesModifiers_)
		{
			entity->addModifier(modifier);
		}
	}

	for(auto& modifier : cannonModifiers_)
	{
		cannon_.addModifier(modifier);
	}
}

void World::cleanEntities()
{
	auto it = entities_.begin();

	while(it != entities_.end())
	{
		auto posPtr = ecs_.getComponent((*it)->getLabel(),
		                                ecs::Component::Position);
		auto collPtr = ecs_.getComponent((*it)->getLabel(),
		                                 ecs::Component::Collidable);
		if(posPtr && collPtr)
		{
			auto posComp = dynCast<ecs::Position>(posPtr);
			assert(posComp);

			auto sphereCollComp = dynCast<ecs::CollidableSphere>(collPtr);
			auto rectCollComp = dynCast<ecs::CollidableRect>(collPtr);

			if((sphereCollComp &&
			    (posComp->position_.y - sphereCollComp->radius_ > originalSize_.y ||
			     posComp->position_.y + sphereCollComp->radius_ < 0 ||
			     posComp->position_.x - sphereCollComp->radius_ > originalSize_.x ||
			     posComp->position_.x + sphereCollComp->radius_ < 0)) ||
			   (rectCollComp &&
			    (posComp->position_.y - rectCollComp->size_.y/2.f > originalSize_.y ||
			     posComp->position_.y + rectCollComp->size_.y/2.f < 0 ||
			     posComp->position_.x - rectCollComp->size_.y/2.f > originalSize_.x ||
			     posComp->position_.x + rectCollComp->size_.y/2.f < 0)))
			{
				entities_.erase(it);
			}

			else
			{
				++it;
			}
		}
	}
}


void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	difficulty_.draw(target, states);

	for(auto it = entities_.begin(); it != entities_.end(); ++it)
	{
		(*it)->draw(target, states);
	}

	cannon_.draw(target, states);
}
