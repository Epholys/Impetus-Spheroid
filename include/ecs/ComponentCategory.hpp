#ifndef ECS_COMPONENTCATEGORY_HPP
#define ECS_COMPONENTCATEGORY_HPP


namespace ecs
{
	namespace Component
	{
		typedef unsigned int CategoryMask;

		enum Category : CategoryMask
		{
			None = 0,
			Position = 1,
			Velocity = 1 << 1,
			Mass = 1 << 2,
			Solid = 1 << 3,
			Collidable = 1 << 4,
			Target = 1 << 5,
			Projectile = 1 << 6,
			CategoryCount = 1 << 7,

			Massic = Velocity | Mass,
			Moveable = Position | Velocity,
		};
	} 
} // namespace ecs



#endif // ECS_COMPONENTCATEGORY_HPP
