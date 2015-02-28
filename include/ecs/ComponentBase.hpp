#ifndef ECS_COMPONENTBASE_HPP
#define ECS_COMPONENTBASE_HPP


#include <memory>

#include "utility/Time.hpp"
#include "ecs/ComponentCategory.hpp"

namespace ecs
{

	/* The base class for all the components of the Entity/Component System, to
	 * allow polymorphism.
	 * Also implements a basic timer system to pause the component for a period
	 * of time.
	 * 
	 * For more information about a Entity/Component System, see:
	 * http://entity-systems.wikidot.com/
	 * */

	class ComponentBase
	{
	public:
		typedef std::shared_ptr<ComponentBase> SPtr;

	public:
		ComponentBase();
		virtual ~ComponentBase();
			
		void update(Time dt);
		void pause(Time t);
		void unpause();
		bool isPaused() const;
		
		virtual Component::Category getCategory() const =0;
		
	private:
		Time pauseCounter_;
	};

} // namespace ecs

#endif // ECS_COMPONENTBASE_HPP
