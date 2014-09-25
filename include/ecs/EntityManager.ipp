

template<typename Comp, typename... Arguments>
void EntityManager::addComponent(Entity ent, Arguments... args)
{
	if(entityExists(ent))
	{
		auto spComp = std::make_shared<Comp>(args...);
		entityComponents_[ent].emplace(spComp->getCategory(), spComp);
		entityMasks_[ent] = entityMasks_[ent] | spComp->getCategory();
	}
}

