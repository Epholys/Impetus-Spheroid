
template<typename Comp, typename... Arguments>
void PhysicEngine::addComponent(Entity ent, Arguments... args)
{
	if(entityExists(ent))
	{
		auto spComp = std::make_shared<Comp>(args...);
		entityComponents_[ent].emplace(spComp->getCategory(), spComp);
	}
}
