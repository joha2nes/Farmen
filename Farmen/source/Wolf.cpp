// Wolf.cpp

#include "Wolf.hpp"
#include "World.hpp"
#include "Sheep.hpp"
#include "Util.hpp"

#include <iostream>

#define walkSpeed 2.0f
#define runSpeed 8.0f

// helpers

void printState(Wolf::State s)
{
	const char* strState =
		s == Wolf::Sleeping ? "sleeping" :
		s == Wolf::Awaking ? "awaking" :
		s == Wolf::Hunting ? "hunting" :
		s == Wolf::Eating ? "eating" :
		s == Wolf::Returning ? "returning" :
		"";
	std::cout << "Wolf is " << strState << std::endl;
}

// Wolf

Wolf::Wolf()
	: m_world(nullptr)
	, m_position({0,0})
	, m_direction({0,0})
	, m_state(Sleeping)
	, m_stateTime(0)
	, m_sheepToEat(nullptr)
	, m_home({0,0})
{}

Wolf::Wolf(World* w, Position p, Direction d, State s)
	: m_world(w)
	, m_position(p)
	, m_direction(d)
	, m_state(s)
	, m_stateTime(0)
	, m_sheepToEat(nullptr)
	, m_home(p)
{}

Wolf::~Wolf()
{
	m_world = nullptr;
	m_sheepToEat = nullptr;
}

void Wolf::update(DeltaTime dt)
{
	m_stateTime += dt;

	switch (m_state)
	{
	case Sleeping:
		if (m_stateTime > 120.0f)
			setState(Awaking);
		break;

	case Awaking:
		findFood();
		if (m_sheepToEat == nullptr)
			setState(Sleeping);
		else
			setState(Hunting);
		break;

	case Hunting:
		m_position = move(m_position, scale(runSpeed * dt, normalize(directionBetween(m_position, m_sheepToEat->getPosition()))));

		if (distanceBetween(m_position, m_sheepToEat->getPosition()) < 1.0f and m_sheepToEat->getState() == Sheep::Dead)
			setState(Eating);
		break;

	case Eating:
		if (m_stateTime > 5.0f)
			setState(Returning);
		break;

	case Returning:
		m_position = move(m_position, scale(walkSpeed * dt, normalize(directionBetween(m_position, m_home))));
		if (distanceBetween(m_position, m_home) < 0.5f)
			setState(Sleeping);
		break;
	}
}

void Wolf::findFood()
{
	const int n = m_world->getSheepCount();
	if (n != 0) {
		// find closest not dead sheep
		std::vector<Sheep*> allSheep = m_world->m_sheep;
		m_sheepToEat = allSheep[0];
		for (int i = 1; i < n; i++)
			if (distanceBetween(m_position, allSheep[i]->getPosition()) < distanceBetween(m_position, m_sheepToEat->getPosition()) and allSheep[i]->getState() != Sheep::Dead)
				m_sheepToEat = allSheep[i];
	}
}

void Wolf::setState(State s)
{
	printState(s);

	m_state = s;
	m_stateTime = 0.0;
	update(0.0);
}

Position Wolf::getPosition() const
{
	return m_position;
}

Direction Wolf::getDirection() const
{
	return m_direction;
}

Wolf::State Wolf::getState() const
{
	return m_state;
}
