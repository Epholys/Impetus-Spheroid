#ifndef STATE_HPP
#define STATE_HPP


#include <memory>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "core/MetaData.hpp"
#include "core/LastGameData.hpp"
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


/* Abstract class which define a State of the program.
 *
 * The Context is like a combination of global variables that all States share.
 * 
 * The only subtility of this class is the boolean returned by update() and
 * handleInput(): if true, it means that the State below it can be updated /
 * handle input. For exemple, a PauseState will not allow the GameState below to
 * be updated, on the contrary of a TutorialState.
 * 
 * */ 

class State
{
public:
	struct Context
	{
		Context(sf::RenderWindow& window, MetaData& metaData, LastGameData& gameData);
		
		sf::RenderWindow* window;
		const Vector2u originalWindowSize;

		MetaData* metaData;
		LastGameData* lastGameData;
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
