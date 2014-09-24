#include "ecs/ComponentBasic.hpp"


namespace ecs
{
	// *** Position: ***

	Position::Position(Vector2f pos)
		: position_(pos)
	{
	}

	Component::Category Position::getCategory() const
	{
		return Component::Position;
	}

//-----------------------------------------------------------------------------
	// *** Velocity: ***

	Velocity::Velocity(Vector2f vel)
		: velocity_(vel)
	{
	}

	Component::Category Velocity::getCategory() const
	{
		return Component::Velocity;
	}

} // namespace ecs
