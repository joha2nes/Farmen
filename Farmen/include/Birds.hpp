// Birds.hpp

#pragma once

#include "Types.hpp"

struct Bird
{
	float xPos, yPos;
	float xDir, yDir;
};

class Birds
{
public:
	Birds(unsigned int numBirds, float xMin, float xMax, float yMin, float yMax);
	~Birds();

	void update(float deltaTime);

	unsigned int getNumOfBirds() const;
	const Bird& getBird(unsigned int) const;

private:
	void cohese(const Bird&, float& xVel, float& yVel);
	void separate(const Bird&, float& xVel, float& yVel);
	void align(const Bird&, float& xVel, float& yVel);

	void boundPosition(const Bird&, float& xVel, float& yVel);

private:
	unsigned int m_numBirds;
	Bird* m_birds;
	float m_xMin, m_xMax;
	float m_yMin, m_yMax;
};