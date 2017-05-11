// Birds.cpp

// references:
// - http://www.kfish.org/boids/pseudocode.html

#include "Birds.hpp"
#include "World.hpp"
#include <math.h>
#include <random>

// definitions

#define separationRadius 1.0f
#define separationForce 2.0f
#define cohesion 1.0f
#define alignment 1.0f
#define speed 5.0f
#define flockRadius 2.0f

// helper functions

#define clamp(a, min, max) a < min ? min : a > max ? max : a
#define magnitude(x, y) sqrt(x * x + y * y)
void normalize(float& x, float& y)
{
	const float mag = magnitude(x, y);
	if (mag > 0.0f)
	{
		x /= mag;
		y /= mag;
	}
}
float distanceBetween(const Bird& b1, const Bird& b2)
{
	const float x = abs(b2.xPos - b1.xPos);
	const float y = abs(b2.yPos - b1.yPos);

	return sqrt(x * x + y * y);
}

// public

Birds::Birds(unsigned int num, float xMin, float xMax, float yMin, float yMax)
	: m_numBirds(num)
	, m_birds(new Bird[num])
	, m_xMin(xMin)
	, m_xMax(xMax)
	, m_yMin(yMin)
	, m_yMax(yMax)
{
	for (int i = 0; i < num; i++)
	{
		const int width = int(xMax - xMin);
		const int height = int(yMax - yMin);
		m_birds[i].xPos = float(rand() % width) + xMin;
		m_birds[i].yPos = float(rand() % height) + yMin;
		m_birds[i].xDir = float(rand() % 20 - 10) / 100;
		m_birds[i].yDir = float(rand() % 20 - 10) / 100;
		normalize(m_birds[i].xDir, m_birds[i].yDir);
	}
}

Birds::~Birds()
{
	delete[] m_birds;
	m_birds = nullptr;
}

void Birds::update(float dt)
{
	float xCohesion, yCohesion;
	float xSeparation, ySeparation;
	float xAlignment, yAlignment;
	float xBounding, yBounding;

	// for each bird...
	for (int i = 0; i < m_numBirds; i++)
	{
		// pass values by reference
		cohese(m_birds[i], xCohesion, yCohesion);
		separate(m_birds[i], xSeparation, ySeparation);
		align(m_birds[i], xAlignment, yAlignment);
		boundPosition(m_birds[i], xBounding, yBounding);

		// calculate wanted direction
		float wantedX = (xCohesion * cohesion + xSeparation * separationForce + xAlignment * alignment + xBounding);
		float wantedY = (yCohesion * cohesion + ySeparation * separationForce + yAlignment * alignment + yBounding);

		normalize(wantedX, wantedY);

		// interpolate from current direction to wanted direction
		m_birds[i].xDir += (wantedX - m_birds[i].xDir) * 0.03f;
		m_birds[i].yDir += (wantedY - m_birds[i].yDir) * 0.03f;
		
		normalize(m_birds[i].xDir, m_birds[i].yDir);

		// move bird
		m_birds[i].xPos += m_birds[i].xDir * speed * dt;
		m_birds[i].yPos += m_birds[i].yDir * speed * dt;
	}
}

unsigned int Birds::getNumOfBirds() const
{
	return m_numBirds;
}

const Bird& Birds::getBird(unsigned int i) const
{
	return m_birds[clamp(i, 0, m_numBirds)];
}

// private

void Birds::cohese(const Bird& bird, float& xCohesion, float& yCohesion)
{
	xCohesion = 0.0f;
	yCohesion = 0.0f;

	// accumulators
	float xMid = 0.0f;
	float yMid = 0.0f;
	int flockSize = 0;

	for (int i = 0; i < m_numBirds; i++)
	{
		if (&m_birds[i] != &bird)
		{
			if (distanceBetween(m_birds[i], bird) < flockRadius)
			{
				xMid += m_birds[i].xPos;
				yMid += m_birds[i].yPos;

				flockSize++;
			}
		}
	}
	if (flockSize != 0)
	{
		xMid /= flockSize;
		yMid /= flockSize;

		xCohesion = (xMid - bird.xPos);
		yCohesion = (yMid - bird.yPos);
	}
}

void Birds::separate(const Bird& bird, float& xSeparation, float& ySeparation)
{
	xSeparation = 0.0f;
	ySeparation = 0.0f;

	float x = 0.0f;
	float y = 0.0f;

	for (int i = 0; i < m_numBirds; i++)
	{
		if (&m_birds[i] != &bird)
		{
			if (distanceBetween(m_birds[i], bird) < separationRadius)
			{
				x += (bird.xPos - m_birds[i].xPos);
				y += (bird.yPos - m_birds[i].yPos);
			}
		}
	}
	xSeparation = x;
	ySeparation = y;
}

void Birds::align(const Bird& bird, float& xAlignment, float& yAlignment)
{
	xAlignment = 0.0f;
	yAlignment = 0.0f;

	// accumulators
	float xVel = 0.0f;
	float yVel = 0.0f;
	int flockSize = 0;

	for (int i = 0; i < m_numBirds; i++)
	{
		if (&m_birds[i] != &bird)
		{
			if (distanceBetween(m_birds[i], bird) < flockRadius)
			{
				xVel += m_birds[i].xDir;
				yVel += m_birds[i].yDir;

				flockSize++;
			}
		}
	}
	if (flockSize != 0)
	{
		xVel /= flockSize;
		yVel /= flockSize;

		xAlignment = (xVel - bird.xDir);
		yAlignment = (yVel - bird.yDir);
	}
}

void Birds::boundPosition(const Bird& bird, float& xDir, float& yDir)
{
	xDir = 0.0f;
	yDir = 0.0f;

	const float breakAmount = 1.0f;

	if (bird.xPos < m_xMin)
		xDir = breakAmount;
	else if (bird.xPos > m_xMax)
		xDir = -breakAmount;
	
	if (bird.yPos < m_yMin)
		yDir = breakAmount;
	else if (bird.yPos > m_yMax)
		yDir = -breakAmount;
}