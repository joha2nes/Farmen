// Util.cpp

#include "Util.hpp"

Position move(Position p, Direction d)
{
	return{ p.x + d.x, p.y + d.y };
}

float distanceBetween(Position p1, Position p2)
{
	return magnitude(directionBetween(p1, p2));
}

Direction directionBetween(Position p1, Position p2)
{
	return{ p2.x - p1.x, p2.y - p1.y };
}

Direction scale(float s, Direction d)
{
	return{ d.x * s, d.y * s };
}

Direction opposite(Direction d)
{
	return{ -d.x, -d.y };
}