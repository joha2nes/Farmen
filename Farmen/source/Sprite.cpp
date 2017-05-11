// Sprite.cpp

#include "Sprite.hpp"

Sprite::Sprite(SDL_Texture* texture)
	: m_texture(texture)
{
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	m_coords = SDL_Rect{ 0, 0, w, h };
	m_center = SDL_Point{ w / 2, h / 2 };
}

Sprite::Sprite(SDL_Texture* texture, SDL_Rect coords)
	: m_texture(texture)
	, m_coords(coords)
	, m_center(SDL_Point{ coords.w / 2, coords.h / 2 })
{}

Sprite::Sprite(SDL_Texture* texture, SDL_Point center)
	: m_texture(texture)
	, m_center(center)
{
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	m_coords = SDL_Rect{ 0, 0, w, h };
}

Sprite::Sprite(SDL_Texture* texture, SDL_Rect coords, SDL_Point center)
	: m_texture(texture)
	, m_coords(coords)
	, m_center(center)
{}

Sprite::~Sprite()
{
	m_texture = nullptr;
}

void Sprite::draw(SDL_Renderer* renderer, SDL_Point point)
{
	draw(renderer, point, 1.0f, SDL_FLIP_NONE);
}

void Sprite::draw(SDL_Renderer* renderer, SDL_Point point, SDL_RendererFlip flip)
{
	draw(renderer, point, 1.0f, flip);
}

void Sprite::draw(SDL_Renderer* renderer, SDL_Point point, float scale)
{
	draw(renderer, point, scale, SDL_FLIP_NONE);
}

void Sprite::draw(SDL_Renderer* renderer, SDL_Point point, float scale, SDL_RendererFlip flip)
{
	SDL_Rect destination = SDL_Rect{ point.x, point.y, int(float(m_coords.w) * scale), int(float(m_coords.h * scale)) };
	SDL_Point center = SDL_Point{ m_center.x, m_center.y };
	SDL_RenderCopyEx(renderer, m_texture, &m_coords, &destination, 0.0f, &center, flip);
}

SDL_Texture* Sprite::getTexture()
{
	return m_texture;
}

SDL_Rect Sprite::getCoords() const
{
	return m_coords;
}

SDL_Point Sprite::getCenter() const
{
	return m_center;
}