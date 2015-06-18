
template<typename T, typename... Args>
void World::addEntity(Args... args)
{
	Entity::Ptr entity (new T (*this, args...));
	entities_.push_back(std::move(entity));
}
