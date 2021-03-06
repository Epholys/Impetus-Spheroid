#include "framework/StateTutorial.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto instructionDatas = genTutorialData();
}


//-----------------------------------------------------------------------------

StateTutorial::StateTutorial(StateStack& stack, Context context)
	: State(stack, context)
	, index_(0)
	, instruction_()
	, arrow_()
{
	instruction_.setFont(context.fonts->get(FontID::ForcedSquare));
	instruction_.setColor(sf::Color::Black);
	instruction_.setCharacterSize(25);

	context.textures->load(TextureID::Arrow, "./media/sprites/Arrow.png");
	arrow_.setTexture(context.textures->get(TextureID::Arrow));
	arrow_.setColor(sf::Color::Black);

	updateInstruction();
}

StateTutorial::~StateTutorial()
{
}

void StateTutorial::updateInstruction()
{
	instruction_.setString(instructionDatas.at(index_).instruction);
	instruction_.setPosition(instructionDatas.at(index_).position);

	arrow_.setPosition(instruction_.findCharacterPos(100) +
	                   Vector2f(instruction_.getCharacterSize() / 2,
	                            instruction_.getCharacterSize() / 2));	
}

//-----------------------------------------------------------------------------

void StateTutorial::draw(sf::RenderStates states)
{
	sf::RenderWindow* window = context_.window;

	window->draw(instruction_, states);
	if(instructionDatas.at(index_).hasArrow)
		window->draw(arrow_, states);
}

bool StateTutorial::update(Time)
{
	return true;
}

bool StateTutorial::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonPressed)
	{
		switch(event.mouseButton.button)
		{
		case sf::Mouse::Left:
		{
			++index_;

			if(index_ < instructionDatas.size())
				updateInstruction();

			else
				requestStackPop();

			break;
		}
		case sf::Mouse::Right:
		{
			index_ = (index_ != 0) ? index_ - 1 : index_;

			updateInstruction();

			break;
		}
		default:
			break;
		}

		return false;
	}

	return true;
}

