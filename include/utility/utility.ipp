
namespace
{
	template<typename... Args>
	void moveArguments(Vector2f, Args& ...)
	{
		// Do Nothing
	}

	template<typename T>
	void moveArguments(Vector2f toMove, T& element)
	{
		element.move(toMove);
	}

	template<typename T, typename... Args>
	void moveArguments(Vector2f toMove, T& first, Args&... args)
	{
		first.move(toMove);
		moveArguments(toMove, args...);
	}
}

template<typename... Args>
void moveGroupOfTexts(Vector2f position, sf::Text& begin, sf::Text& end, Args&... args)
{
	Vector2f centerOfTexts ((end.findCharacterPos(end.getString().getSize()) - begin.findCharacterPos(0)) / 2.f);
	Vector2f toMove = position - centerOfTexts;
	begin.move(toMove);
	end.move(toMove);
	moveArguments(toMove, args...);
}
