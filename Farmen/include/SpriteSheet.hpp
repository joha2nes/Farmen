// SpriteSheet.hpp

#pragma once

#include <SDL.h>

class Sprite;

class SpriteSheet
{
public:
	SpriteSheet(SDL_Texture*, unsigned int tileSize);
	~SpriteSheet();

	Sprite getSprite(unsigned int x, unsigned int y);

private:
	SDL_Texture* m_texture;
	unsigned int m_rows;
	unsigned int m_columns;
	unsigned int m_tileSize;
};
