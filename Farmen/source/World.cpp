// World.cpp

#include "World.hpp"
#include "Tile.hpp"
#include "Sheep.hpp"
#include "Wolf.hpp"
#include "Birds.hpp"
#include <random>

World::World(unsigned int width, unsigned int height, unsigned int numberOfSheep)
	: m_width(width)
	, m_height(height)
	, m_size(width * height)
	, m_sheepCount(numberOfSheep)
{
	// create earth
	m_tiles = new Tile[width * height];
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {
			float grass = rand() % 40 + 0;
			float fertility = rand() % 20 + 10;
			m_tiles[y * width + x] = Tile(x, y, grass, fertility, Tile::Growing);
		}
	// create sheep
	//m_sheep = new Sheep[numberOfSheep];
	for (int i = 0; i < numberOfSheep; i++) {
		float x = float(rand() % width);
		float y = float(rand() % height);
		Sheep* s = new Sheep(this, {x, y}, {1, 0}, Sheep::Walking);
		m_sheep.push_back(s);
	}
	// create wolf
	m_wolf = new Wolf(this, { -20, 5 }, { 1, 0 }, Wolf::Sleeping);
	// create birds
	m_birds = new Birds(40, float(0), float(m_width), float(0), float(m_height));
}

World::~World()
{
	delete[] m_tiles;
	m_tiles = nullptr;
	//delete[] m_sheep;
	//m_sheep = nullptr;
	delete m_wolf;
	m_wolf = nullptr;
	delete m_birds;
	m_birds = nullptr;
}

void World::update(float dt)
{
	for (int i = 0; i < m_size; i++)
		m_tiles[i].update(dt);
	for (int i = 0; i < m_sheepCount; i++)
		m_sheep[i]->update(dt);
	m_wolf->update(dt);
	m_birds->update(dt);
}

const Tile* World::getTiles() const
{
	return m_tiles;
}

unsigned int World::getWidth() const
{
	return m_width;
}

unsigned int World::getHeight() const
{
	return m_height;
}

unsigned int World::getSize() const
{
	return m_size;
}

const std::vector<Sheep*> World::getSheep() const
{
	return m_sheep;
}

unsigned int World::getSheepCount() const
{
	return m_sheepCount;
}

const Wolf* World::getWolf() const
{
	return m_wolf;
}

const Birds* World::getBirds() const
{
	return m_birds;
}