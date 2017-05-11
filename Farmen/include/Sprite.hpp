// Sprite.hpp

#pragma once

#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Texture* texture);
	Sprite(SDL_Texture* texture, SDL_Rect coords);
	Sprite(SDL_Texture* texture, SDL_Point center);
	Sprite(SDL_Texture* texture, SDL_Rect coords, SDL_Point center);
	virtual ~Sprite();

	void draw(SDL_Renderer*, SDL_Point);
	void draw(SDL_Renderer*, SDL_Point, SDL_RendererFlip);
	void draw(SDL_Renderer* renderer, SDL_Point point, float scale);
	void draw(SDL_Renderer*, SDL_Point, float scale, SDL_RendererFlip);

	SDL_Texture* getTexture();
	SDL_Rect getCoords() const;
	SDL_Point getCenter() const;

protected:
	SDL_Texture* m_texture;
	SDL_Rect m_coords;
	SDL_Point m_center;
};
