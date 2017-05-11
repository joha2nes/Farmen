// World.hpp

#pragma once

#include <vector>

class Tile;
class Sheep;
class Wolf;
class Birds;

class World
{
	friend Sheep;
	friend Wolf;
public:
	World(unsigned int width, unsigned int height, unsigned int numberOfSheep);
	~World();

	void update(float dt);

	const Tile* getTiles() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getSize() const;
	const std::vector<Sheep*> getSheep() const;
	unsigned int getSheepCount() const;
	const Wolf* getWolf() const;
	const Birds* getBirds() const;

private:
	Tile* m_tiles;
	const unsigned int m_width;
	const unsigned int m_height;
	const unsigned int m_size;
	std::vector<Sheep*> m_sheep;
	//Sheep* m_sheep;
	const unsigned int m_sheepCount;
	Wolf* m_wolf;
	Birds* m_birds;
};