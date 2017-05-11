// Tile.hpp

#pragma once

#include "Types.hpp"

#define TILE_MAX_GRASS 100.0f
#define TILE_MAX_FERTILITY 100.0f

class Sheep;

class Tile
{
	friend Sheep;
public:
	enum State
	{
		Growing,
		Fullgrown,
		Withering,
		Decomposed
	};

public:
	Tile();
	Tile(unsigned int x, unsigned int y, float grass, float fertility, State initialState);
	~Tile();

	void update(float dt);

	void setState(State);
	void setFertility(float);

	Position getPosition() const;
	unsigned int getX() const;
	unsigned int getY() const;
	float getGrass() const;
	float getFertility() const;
	State getState() const;

private:
	Position m_position;
	float m_grass;
	float m_fertility;
	State m_state;
	float m_stateTime;
};
