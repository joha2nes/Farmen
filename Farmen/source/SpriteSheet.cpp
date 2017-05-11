// SpriteSheet.cpp

#include "SpriteSheet.hpp"
#include "Sprite.hpp"

#define clamp(x, a, b) x < a ? a : x > b ? b : x

SpriteSheet::SpriteSheet(SDL_Texture* texture, unsigned int tileSize)
	: m_texture(texture)
	, m_tileSize(tileSize)
{
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	m_rows = h / tileSize;
	m_columns = w / tileSize;
}

SpriteSheet::~SpriteSheet()
{}

Sprite SpriteSheet::getSprite(unsigned int x, unsigned int y)
{
	x = clamp(x, 1, m_columns + 1);
	y = clamp(y, 1, m_rows + 1);
	return Sprite(m_texture, SDL_Rect{ ((int)x - 1) * (int)m_tileSize, ((int)y - 1) * (int)m_tileSize, (int)m_tileSize, (int)m_tileSize });
}
