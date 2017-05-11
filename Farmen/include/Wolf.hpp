// Wolf.hpp

#pragma once

#include "Types.hpp"

class World;
class Sheep;

class Wolf
{
public:
	enum State
	{
		Sleeping,
		Awaking,
		Hunting,
		Eating,
		Returning
	};

public:
	Wolf();
	Wolf(World*, Position, Direction, State);
	~Wolf();

	void update(DeltaTime);

	void setState(State);

	Position getPosition() const;
	Direction getDirection() const;
	State getState() const;

private:
	void findFood();

private:
	World* m_world;
	Position m_position;
	Direction m_direction;
	State m_state;
	DeltaTime m_stateTime;
	Sheep* m_sheepToEat;
	Position m_home;
};