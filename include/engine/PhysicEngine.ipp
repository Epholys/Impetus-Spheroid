
template<typename Child>
std::shared_ptr<Child> PhysicEngine::dynCast(const ecs::ComponentBase::SPtr& pComp)
{
	return std::dynamic_pointer_cast<Child>(pComp);
}
