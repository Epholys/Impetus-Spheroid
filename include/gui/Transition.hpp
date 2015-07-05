#ifndef GUI_TRANSITION
#define GUI_TRANSITION


#include <functional>

#include <SFML/Graphics/Transformable.hpp>

#include "utility/Vector2.hpp"
#include "utility/Time.hpp"

namespace gui
{

	struct TransformData
	{
		TransformData(Vector2f position = Vector2f(0.f,0.f),
		              float angle = 0.f,
		              Vector2f scale = Vector2f(1.f, 1.f))
			: position(position), angle(angle), scale(scale)
			{}				   
				
		Vector2f position;
		float angle;
		Vector2f scale;
	};
	TransformData operator+(const TransformData& left, const TransformData& right);
	TransformData operator*(const TransformData& left, float right);	
	
	class Transition
	{
	public:
		enum Type
		{
			Linear
		};
		
	public:
		Transition(sf::Transformable* toMove,
		           Type type,
		           const TransformData& begin,
		           const TransformData& end,
		           Time duration);

		void update(Time dt);
		void updateFinish(const TransformData& newFinish, Time toAdd = Time());

		bool isOver() const;
		TransformData getStart() const;
		TransformData getFinish() const;
		Time getAccumulatedTime() const;
		Time getDuration() const;

	private:
		void setTransformation(const TransformData& data);
		void applyTransformation(const TransformData& data, Time dt);
		
	private:
		sf::Transformable* toMove_;
		Type type_;
		TransformData start_;
		TransformData finish_;
		Time duration_;
		Time accumulatedTime_;
		std::function<TransformData(Time)> transitionFunction_;
	};
}


#endif // GUI_TRANSITION
