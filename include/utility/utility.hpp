#ifndef UT_UTILITY_HPP
#define UT_UTILITY_HPP


#include <memory>

#include "ecs/ComponentBase.hpp"

/* Just to have a shorter name that
 * std::dynamic_pointer_cast<...>(ecs::ComponentBase)   
 * */
template<typename Child>
std::shared_ptr<Child> dynCast(const ecs::ComponentBase::SPtr& pParent)
{
	return std::dynamic_pointer_cast<Child>(pParent);
}


#endif // UT_UTILITY_HPP
