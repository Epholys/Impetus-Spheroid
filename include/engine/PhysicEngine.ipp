
template<typename Comp, typename... Arguments>
void PhysicEngine::addComponent(Entity ent, Arguments... args)
{
	if(entityExists(ent))
	{
		entityComponents_[ent].push_back(std::make_shared<Comp>(args));
	}
}
