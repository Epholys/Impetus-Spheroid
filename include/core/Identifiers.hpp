#ifndef IDENTIFIERS_HPP
#define IDENTIFIERS_HPP


namespace EntityID
{
	enum Type  : unsigned int
	{
		None = 0,
		Wall = 1,
		Ball = 1 << 1,
		Target = 1 << 2,
	};
}


#endif // IDENTIFIERS_HPP
