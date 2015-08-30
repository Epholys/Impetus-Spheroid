#include "core/World.hpp"
#include "core/MetaData.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------
// *** constructor: ***

namespace
{
	const std::vector<Vector2f> CANNON_POSITION { {40.f, 580.f}, {760.f, 580.f} };
	const std::vector<Vector2f> CANNON_BALLS_POSITION { {2.5f, 560.f}, {777.5f, 560.f} };
}

//const Time World::TIME_BEETWEEN_FIRE = milliseconds(333);
// Vector2f World::CANON_POSITION {40.f, 580.f};


World::World(const Vector2u& originalSize,
             MetaData& metaData,
             FontHolder& fonts,
             TextureHolder& textures,
             int precision)
	: originalSize_(originalSize)
	, font_(fonts.get(FontID::ForcedSquare))
	, textures_(textures)
	, metaData_(metaData)
	, ecs_()
	, physEng_(ecs_, precision)
	, evtGen_()
	, difficulty_(DifficultyContext{this, &evtGen_, &metaData, &(metaData.inventory)})
	, inventory_(metaData.inventory)
	, cannons_()
	, nCannon_(0)
	, state_(Waiting)
	, speedCoeff_(1.f)
	, gravityVect_(0.f, 1000.f)
	, entities_()
	, ceiling_(nullptr)
	, leftWall_(nullptr)
	, targetHighlight_(Vector2f(3* originalSize_.x / 4.f, originalSize_.y / 2.f),
	                   sf::FloatRect(0.f, 0.f, originalSize_.x, originalSize_.y))
	, particleSystems_()
	, otherDrawings_()
{
	addCannon();
	
	for(int i=0; i<Particle::TypeCount; ++i)
	{
		particleSystems_.push_back(ParticleSystem(Particle::Type(i)));
	}
	
	generateWorld();
}

void World::generateWorld()
{
	Wall* ceiling  = new Wall(*this,
	                          Vector2f(originalSize_.x / 2.f, 0.f),
	                          Vector2f(originalSize_.x, 10.f),
	                          sf::Color(80,80,80));
	
	Wall* leftWall = new Wall(*this,
	                          Vector2f(12.5f, originalSize_.y / 2.f),
	                          Vector2f(25.f, originalSize_.y),
	                          sf::Color(80,80,80));

	Entity::Ptr rightWall (new Wall(*this,
	                                Vector2f(originalSize_.x - 12.5f, originalSize_.y / 2.f),
	                                Vector2f(25.f, originalSize_.y),
	                                sf::Color(80,80,80)));

	ceiling_ = ceiling;
	leftWall_ = leftWall;
	
	entities_.push_back(std::move(Entity::Ptr(ceiling)));
	entities_.push_back(std::move(Entity::Ptr(leftWall)));
	entities_.push_back(std::move(rightWall));
	
	for(const auto& ent : entities_)
	{
		ent->update(Time());
	}

	createTarget(Vector2f(3* originalSize_.x / 4.f, originalSize_.y / 2.f));
}

unsigned int World::getNCannon() const
{
	return nCannon_;
}

void World::addCannon()
{
	if(nCannon_ < CANNON_POSITION.size())
	{
		auto newCannon = std::make_shared<Cannon>(CANNON_POSITION.at(nCannon_),
		                                          CANNON_BALLS_POSITION.at(nCannon_),
		                                          *this,
		                                          inventory_,
		                                          metaData_.improvementValue[ImprovementID::BallsPerSecond]);
		cannons_.push_back(newCannon);
	}
	++nCannon_;
}

void World::removeCannon()
{
	--nCannon_;
	if(nCannon_ < CANNON_POSITION.size())
	{
		cannons_.pop_back();
	}
}

//-----------------------------------------------------------------------------

sf::Font& World::getFontRef()
{
	return font_;
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

ParticleSystem& World::getParticleSystem(Particle::Type type)
{
	return particleSystems_.at(type);
}

Vector2f World::getGravityVect() const
{
	return gravityVect_;
}

void World::setGravityVect(Vector2f vect)
{
	gravityVect_ = vect;
}

Vector2u World::getWindowSize() const
{
	return Vector2u(originalSize_);
}
Vector2f World::getMousePosition() const
{
	return mousePosition_;
}

Wall * World::getCeiling()
{
	return ceiling_;
}
Wall * World::getLeftWall()
{
	return leftWall_;
}


template<>
void World::forwardModifier<World>(Modifier<World> modifier)
{		
	Modifiable<World>::addModifier(modifier);
}


template<>
void World::forwardModifier<Entity>(Modifier<Entity> modifier)
{
	for (auto& entity : entities_)
	{
		entity->addModifier(modifier);
	}
}

template<>
void World::forwardModifier<Cannon>(Modifier<Cannon> modifier)
{		
	for(auto& cannon : cannons_)
	{
		cannon->addModifier(modifier);
	}
}

void World::addEntity(Entity::Ptr entity)
{
	entities_.push_back(std::move(entity));
}

void World::removeEntity(ecs::Entity label)
{
	// By security, even if Entity's destructor do it
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

bool World::hasStarted() const
{
	return state_ != Waiting && state_ != WaitingTutorial;
}

void World::setState(GameState state)
{
	state_ = state;
}

void World::cancelEvents(bool comeFromInventory)
{
	Modifiable<World>::forceEndingModifiers(*this);

	for (auto& entity : entities_)
	{
		entity->forceEndingModifiers(*entity);
	}
	
	for(auto& cannon : cannons_)
	{
		cannon->forceEndingModifiers(*cannon);
	}

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
		for(auto& cannon : cannons_)
		{
			cannon->updateTubeDirection();
		}

		if(state_ == Waiting || state_ == WaitingTutorial)
		{
			for(auto& cannon : cannons_)
			{
				cannon->updateArcPreview();
			}
		}
	}

	else if(state_ == Waiting || state_ == WaitingTutorial)
	{
		if(event.type == sf::Event::MouseButtonPressed)
		{
			state_ = Playing;
			mousePosition_ = Vector2f(event.mouseButton.x, event.mouseButton.y);
		}
		else
			return;
	}

	for(auto& cannon : cannons_)
	{
		cannon->handleInput(event);
	}
}

void World::update(Time dt)
 {
	 if(state_ == WaitingTutorial)
	    return;
	    
	if(state_ == Waiting)
	{
		targetHighlight_.update(dt);
		return;
	}

	// All the objects that requires the "real" time:
	if(state_ != GameOver)
	{
		difficulty_.update(dt);
	}

	getEvent(dt);

	// All the objects that requires the faster time below:
	dt *= speedCoeff_;

	if(!(state_ == Waiting || state_ == GameOver))
	{
		for(auto& cannon : cannons_)
		{
			cannon->update(dt);
		}
	}

	ecs_.update(dt);
	applyModifiers(*this, dt);

	for(const auto& ent : entities_)
	{
		ent->update(dt);
	}
	physEng_.update(dt);
	for(auto& system : particleSystems_)
	{
		system.update(dt);
	}

	for(auto& entry : otherDrawings_)
	{
		entry.transitions.update(dt);
		entry.fadeOut.update(dt);
	}
	
	cleanEntities();
	cleanModifiers();
	cleanOtherDrawings();
}


void World::getEvent(Time dt)
{
	evt::Event evt = evtGen_.update(dt);

	if(evt.diff == evt::Event::None)
		return;

	if(!evt.worldModifiers.empty())
	{
		modifiers_.insert(modifiers_.begin(),
		                  evt.worldModifiers.begin(),
		                  evt.worldModifiers.end());
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

			auto collComp = dynCast<ecs::Collidable>(collPtr);

			if((collComp->type == ecs::Collidable::Sphere &&
			    (posComp->position_.y - collComp->radius > originalSize_.y ||
			     posComp->position_.y + collComp->radius < 0 ||
			     posComp->position_.x - collComp->radius > originalSize_.x ||
			     posComp->position_.x + collComp->radius < 0)) ||
			   (collComp->type == ecs::Collidable::Rectangle &&
			    (posComp->position_.y - collComp->size.y/2.f > originalSize_.y ||
			     posComp->position_.y + collComp->size.y/2.f < 0 ||
			     posComp->position_.x - collComp->size.y/2.f > originalSize_.x ||
			     posComp->position_.x + collComp->size.y/2.f < 0)))
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

void World::cleanOtherDrawings()
{
	auto itEnd =
		std::remove_if(otherDrawings_.begin(),
		               otherDrawings_.end(),
		               [](const DrawingEntry& entry)
		               {
			               return entry.transitions.isOver();
		               });

	otherDrawings_.erase(itEnd, otherDrawings_.end());
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(state_ == Waiting || state_ == WaitingTutorial)
	{
		target.draw(targetHighlight_, states);
	}

	std::for_each(otherDrawings_.cbegin(),
	              otherDrawings_.cend(),
	              [&target, states](const DrawingEntry& entry)
	              {
		              if(!entry.isInFront)
			              target.draw(entry.sprite, states);
	              });
	
	for(const auto& system : particleSystems_)
	{
		target.draw(system, states);
	}

	for(auto it = entities_.begin(); it != entities_.end(); ++it)
	{
		(*it)->draw(target, states);
	}

	for(const auto& cannon : cannons_)
	{

		cannon->draw(target, states);
	}
		
	difficulty_.draw(target, states);

	std::for_each(otherDrawings_.cbegin(),
	              otherDrawings_.cend(),
	              [&target, states](const DrawingEntry& entry)
	              {
		              if(entry.isInFront)
			              target.draw(entry.sprite, states);
	              });
}


//-----------------------------------------------------------------------------

void World::addSprite(TextureID::ID id, const std::string& path, sf::Color color, gui::MultiTransition transitions, bool fadeOut, bool isInFront)
{
	textures_.load(id, path);

	sf::Sprite sprite (textures_.get(id));
	centerOrigin(sprite);
	sprite.setColor(color);

	otherDrawings_.push_back({sprite, transitions, {nullptr, Time(), Time()}, isInFront});
	otherDrawings_.back().transitions.setTransformable(&otherDrawings_.back().sprite);
	
	if(fadeOut)
	{
		otherDrawings_.back().fadeOut = {&otherDrawings_.back().sprite,
		                                 otherDrawings_.back().transitions.getDuration(),
		                                 Time()};
	}

	// Necessary, as std::vector may change its element's address in memory
	for(auto& entry : otherDrawings_)
	{
		entry.transitions.setTransformable(&entry.sprite);
		if(fadeOut)
			entry.fadeOut.pT_ = &entry.sprite;
	}
}
