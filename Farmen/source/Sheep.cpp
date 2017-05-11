// Sheep.cpp

#include "Sheep.hpp"
#include "World.hpp"
#include "Tile.hpp"
#include "Wolf.hpp"
#include "Util.hpp"
#include <random>

#include <iostream>

#define walkSpeed 0.5f
#define runSpeed 4.0f
#define fearDistance 13.0f

// helpers

void printState(Sheep::State s)
{
	const char* strState =
		s == Sheep::Sleeping ? "sleeping" :
		s == Sheep::Walking ? "walking" :
		s == Sheep::Eating ? "eating" :
		s == Sheep::Fleeing ? "fleeing" :
		s == Sheep::Shitting ? "shitting" :
		"";
	std::cout << "Sheep is " << strState << std::endl;
}

// Sheep

Sheep::Sheep()
	: m_world(nullptr)
	, m_position({0.0, 0.0})
	, m_direction({0.0, 0.0})
	, m_state(Walking)
	, m_foodInStomach(50.0f)
{}

Sheep::Sheep(World* w, Position p, Direction d, State s)
	: m_world(w)
	, m_position(p)
	, m_direction(d)
	, m_state(s)
	, m_stateTime(0.0)
	, m_foodInStomach(50.0f)
{
	findFood();
}

Sheep::~Sheep()
{}

void Sheep::update(float dt)
{
	m_stateTime += dt;
	
	switch (m_state)
	{
	case Walking:
	{
		m_direction = normalize(directionBetween(m_position, m_tileToEat->getPosition()));
		m_position = move(m_position, scale(walkSpeed * dt, m_direction));

		keepInBounds();
		fearDangers();

		m_foodInStomach -= dt * 0.02f;

		if (distanceBetween(m_position, m_tileToEat->getPosition()) < 0.5f)
			setState(Eating);
	}
	break;

	case Eating:
	{
		fearDangers();

		if (m_tileToEat->getGrass() < 10.0f or m_tileToEat->getState() == Tile::Decomposed) {
			findFood();
			setState(Walking);
		}
		else
		{
			m_tileToEat->m_grass -= 3.0f * dt;
			m_foodInStomach += 3.0f * dt;
			if (m_foodInStomach >= 100.0f)
				setState(Shitting);
		}
	}
	break;

	case Shitting:
	{
		const float shitTime = 1.0f;
		m_foodInStomach -= 20.0f * m_stateTime / shitTime;

		const int index = int(round(m_position.x) * round(m_position.x) + round(m_position.x));
		m_world->m_tiles[index].setFertility(100.0f);

		if (m_stateTime >= shitTime)
			setState(Walking);
	}
	break;

	case Sleeping:
	{
		fearDangers();
	}
	break;

	case Fleeing:
	{
		m_direction = normalize(opposite(directionBetween(m_position, m_world->m_wolf->getPosition())));
		m_position = move(m_position, scale(runSpeed * dt, m_direction));
		keepInBounds();
		if (distanceBetween(m_position, m_world->m_wolf->getPosition()) < 1.0f)
			setState(Dead);
		else if (distanceBetween(m_position, m_world->m_wolf->getPosition()) > fearDistance + 3.0f) {
			findFood();
			setState(Walking);
		}
	}
	break;
	}
}

void Sheep::findFood()
{
	m_tileToEat = &m_world->m_tiles[rand() % m_world->getSize()];
}

void Sheep::fearDangers()
{
	if (distanceBetween(m_position, m_world->m_wolf->getPosition()) <= fearDistance)
		setState(Fleeing);
}

void Sheep::keepInBounds()
{
	m_position.x =
		m_position.x >= m_world->getWidth() ? m_world->getWidth() :
		m_position.x <= 0 ? 0 :
		m_position.x;
	m_position.y =
		m_position.y >= m_world->getHeight() ? m_world->getHeight() :
		m_position.y <= 0 ? 0 :
		m_position.y;
}

void Sheep::setState(State s)
{
	//printState(s);

	m_state = s;
	m_stateTime = 0.0;
	update(0.0);
}

Position Sheep::getPosition() const
{
	return m_position;
}

Direction Sheep::getDirection() const
{
	return m_direction;
}

Sheep::State Sheep::getState() const
{
	return m_state;
}

float Sheep::getFoodInStomach() const
{
	return m_foodInStomach;
}
