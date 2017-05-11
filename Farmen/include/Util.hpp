// Util.hpp

#pragma once

#include "Types.hpp"
#include <math.h>

#define clamp(a, min, max) a < min ? min : a > max ? max : a
#define square(x) x * x
#define magnitude(v) sqrt(square(v.x) + square(v.y))
#define normalize(v) { v.x / magnitude(v), v.y / magnitude(v) }
#define and &&
#define or ||
#define not !

Position move(Position, Direction);
float distanceBetween(Position, Position);
Direction opposite(Direction);
Direction directionBetween(Position, Position);
Direction scale(float, Direction);
