// Sheep.hpp

#pragma once

#include "Types.hpp"

class World;
class Tile;
class Wolf;

class Sheep
{
public:
	enum State
	{
		Walking,
		Eating,
		Shitting,
		Sleeping,
		Fleeing,
		Dead
	};

public:
	Sheep();
	Sheep(World*, Position, Direction, State);
	~Sheep();

	void update(float dt);

	void setState(State);

	Position getPosition() const;
	Direction getDirection() const;
	State getState() const;
	float getFoodInStomach() const;

private:
	void findFood();
	void fearDangers();
	void keepInBounds();

private:
	World* m_world;
	Position m_position;
	Direction m_direction;
	State m_state;
	float m_stateTime;
	Tile* m_tileToEat;
	float m_foodInStomach;
};