#ifndef ECS_COMPONENTCATEGORY_HPP
#define ECS_COMPONENTCATEGORY_HPP


namespace ecs
{
	namespace Component
	{
		enum Category : unsigned int
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
		};
	} 
} // namespace ecs



#endif // ECS_COMPONENTCATEGORY_HPP
