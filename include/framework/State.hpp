#ifndef STATE_HPP
#define STATE_HPP


#include <memory>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "core/TransGamesData.hpp"
#include "utility/Time.hpp"

namespace sf
{
	class RenderWindow;
}

class StateStack;



namespace StateID
{
	enum ID
	{
		None = 0,
		Game,
		GameOver,
		Pause,
		Market,
	};
}


class State
{
public:
	struct Context
	{
		Context(sf::RenderWindow& window, TransGamesData& datas);
		
		sf::RenderWindow* window;
		const Vector2f originalWindowSize;
		TransGamesData* datas;
	};

public:
	typedef std::unique_ptr<State> UPtr;

public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw(sf::RenderStates states) =0;
	virtual bool update(Time dt) =0;
	virtual bool handleInput(const sf::Event& event) =0;

protected:
	void requestStackPush(StateID::ID id);
	void requestStackPop();
	void requestStackClear();

protected:
	StateStack* stack_;
	Context context_;
};


#endif // STATE_HPP
