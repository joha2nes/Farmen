// Tile.cpp

#include "Tile.hpp"
#include "Util.hpp"

#include <iostream>

Tile::Tile()
	: m_position({ 0.0f, 0.0f })
	, m_grass(0.0)
	, m_fertility(0.0)
	, m_state(Decomposed)
	, m_stateTime(0.0)
{}

Tile::Tile(unsigned int x, unsigned int y, float grass, float fertility, State initialState)
	: m_position({ float(x), float(y) })
	, m_grass(clamp(grass, 0, TILE_MAX_GRASS))
	, m_fertility(clamp(fertility, 0, TILE_MAX_FERTILITY))
	, m_state(initialState)
	, m_stateTime(0)
{}

Tile::~Tile()
{}

// public

void Tile::update(float dt)
{
	m_stateTime += dt;

	switch (m_state)
	{
	case Growing:
		m_grass += m_fertility * dt * 0.003;
		if (m_grass >= TILE_MAX_GRASS)
			setState(Fullgrown);
		break;
	case Fullgrown:
		m_grass = clamp(m_grass, 0.0f, TILE_MAX_GRASS);
		if (m_stateTime >= 240.0)
			setState(Withering);
		else if (m_grass < TILE_MAX_GRASS)
			setState(Growing);
		break;
	case Withering:
		m_grass -= m_fertility * dt * 0.005;
		if (m_grass <= 0.0)
			setState(Decomposed);
		break;
	case Decomposed:
		m_grass = 0.0;
		break;
	}

	m_fertility -= m_fertility * 0.002 * dt;
}

// private

void Tile::setState(State newState)
{
	m_state = newState;
	m_stateTime = 0;
	update(0.0);
}

// setters , getters

void Tile::setFertility(float f)
{
	m_fertility = f;
}

Position Tile::getPosition() const
{
	return m_position;
}

unsigned int Tile::getX() const
{
	return unsigned int(m_position.x);
}
	
unsigned int Tile::getY() const
{
	return unsigned int(m_position.y);
}

float Tile::getGrass() const
{
	return m_grass;
}

float Tile::getFertility() const
{
	return m_fertility;
}

Tile::State Tile::getState() const
{
	return m_state;
}
