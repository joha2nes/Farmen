// Main.cpp

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <cassert>
#include <random>
#include <time.h>
#include <iostream>

#include "World.hpp"
#include "Tile.hpp"
#include "Sheep.hpp"
#include "Wolf.hpp"
#include "Birds.hpp"

#include "Util.hpp"
#include "Sprite.hpp"
#include "SpriteSheet.hpp"

// defines
#define timeScale 1.0
#define windowTitle "Some game"
#define windowWidth 800
#define windowHeight 600
#define drawColor 210, 230, 255, 255
#define tileSize 32

// global variables
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Texture* g_texture = nullptr;
SDL_Texture* g_sheepTexture = nullptr;
SDL_Texture* g_wolfTexture = nullptr;
SDL_Texture* g_backgroundTexture = nullptr;
SDL_Texture* g_birdTexture = nullptr;
Mix_Music* g_ambience = nullptr;
World* g_world = nullptr;

// functions

bool pollEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
		if (e.type == SDL_QUIT)
			return false;
	return true;
}

void display()
{
	// clear
	SDL_SetRenderDrawColor(g_renderer, drawColor);
	SDL_RenderClear(g_renderer);
	
	// draw background

	Sprite background(g_backgroundTexture, SDL_Point{ 0, 0 });
	background.draw(g_renderer, { 0, 0 });
	
	// draw tiles

	const int xOffset = 30;
	const int yOffset = 300;

	SpriteSheet spriteSheet(g_texture, tileSize);

	for (int i = 0; i < g_world->getSize(); i++)
	{
		const Tile* t = &g_world->getTiles()[i];

		const int x = t->getX() * tileSize + t->getY() * tileSize * 0.2 + xOffset;
		const int y = t->getY() * tileSize * 0.3 + yOffset;
		const float f = t->getFertility();
		const float g = t->getGrass();

		switch (t->getState())
		{
		case Tile::Growing:
			g < 12.5 ? spriteSheet.getSprite(7, 4).draw(g_renderer, { x, y }) :
			g < 25.0 ? spriteSheet.getSprite(9, 6).draw(g_renderer, { x, y }) :
			g < 37.5 ? spriteSheet.getSprite(10, 6).draw(g_renderer, { x, y }) :
			g < 50.0 ? spriteSheet.getSprite(11, 6).draw(g_renderer, { x, y }) :
			g < 62.5 ? spriteSheet.getSprite(12, 6).draw(g_renderer, { x, y }) :
			g < 75.0 ? spriteSheet.getSprite(13, 6).draw(g_renderer, { x, y }) :
			g < 87.5 ? spriteSheet.getSprite(14, 6).draw(g_renderer, { x, y }) :
			g < 100.0 ? spriteSheet.getSprite(15, 6).draw(g_renderer, { x, y }) :
			0;
		break;

		case Tile::Fullgrown:
			spriteSheet.getSprite(16, 6).draw(g_renderer, { x, y });
		break;
		
		case Tile::Withering:
			g > 86.0 ? spriteSheet.getSprite(16, 9).draw(g_renderer, { x, y }) :
			g > 72.0 ? spriteSheet.getSprite(16, 7).draw(g_renderer, { x, y }) :
			g > 58.0 ? spriteSheet.getSprite(15, 7).draw(g_renderer, { x, y }) :
			g > 44.0 ? spriteSheet.getSprite(14, 7).draw(g_renderer, { x, y }) :
			g > 30.0 ? spriteSheet.getSprite(13, 7).draw(g_renderer, { x, y }) :
			g > 16.0 ? spriteSheet.getSprite(12, 7).draw(g_renderer, { x, y }) :
			g > 0.0  ? spriteSheet.getSprite(11, 7).draw(g_renderer, { x, y }) :
			0;
			break;
		
		case Tile::Decomposed:
			// ~~
			break;
		}
	}

	// draw sheep

	SpriteSheet sheepsheet(g_sheepTexture, 32);

	for (int i = 0; i < g_world->getSheepCount(); i++)
	{
		const Sheep* s = g_world->getSheep()[i];
		const int x = s->getPosition().x * tileSize + s->getPosition().y * tileSize * 0.2 + xOffset;
		const int y = s->getPosition().y * tileSize * 0.3 + yOffset;
		const float dirX = s->getDirection().x;
		const float dirY = s->getDirection().y;
		switch (s->getState())
		{
		case Sheep::Dead:
			sheepsheet.getSprite(2, 5).draw(g_renderer, { x, y });
			break;
		case Sheep::Shitting:
			sheepsheet.getSprite(1, 5).draw(g_renderer, { x, y });
			break;
		default:
			dirX >= 0.5 ? sheepsheet.getSprite(2, 3).draw(g_renderer, { x, y }) :
			dirX <= -0.5 ? sheepsheet.getSprite(2, 2).draw(g_renderer, { x, y }) :
			dirY >= 0.5 ? sheepsheet.getSprite(2, 1).draw(g_renderer, { x, y }) :
			dirY <= -0.5 ? sheepsheet.getSprite(2, 4).draw(g_renderer, { x, y }) :
			0;
			break;
		}
	}

	// draw wolf
	{
		SpriteSheet wolfsheet(g_wolfTexture, 140);

		const Wolf* w = g_world->getWolf();
		const int x = w->getPosition().x * tileSize + w->getPosition().y * tileSize * 0.2 + xOffset;
		const int y = w->getPosition().y * tileSize * 0.3 + yOffset;
		const float& dirX = w->getDirection().x;
		const float& dirY = w->getDirection().y;
		if (dirX >= 0.1f)
			wolfsheet.getSprite(3, 2).draw(g_renderer, { x, y }, 0.6f, SDL_FLIP_NONE);
		else if (dirX <= 0.1f)
			wolfsheet.getSprite(3, 2).draw(g_renderer, { x, y }, 0.6f, SDL_FLIP_HORIZONTAL);
	}

	// draw birds
	{
		SpriteSheet birdsheet(g_birdTexture, 32);

		const Birds* birds = g_world->getBirds();

		for (int i = 0; i < birds->getNumOfBirds(); i++)
		{
			const int x = int(birds->getBird(i).xPos * tileSize + birds->getBird(i).yPos * tileSize * 0.2 + xOffset);
			const int y = int(birds->getBird(i).yPos * tileSize * 0.3 + yOffset - 200.0f);
			//const int x = int(birds->getBird(i).xPos * tileSize);// +birds->getBird(i).yPos * tileSize * 0.2 + xOffset);
			//const int y = int(birds->getBird(i).yPos * tileSize);// *0.3 + yOffset - 100.0f);
			const float vx = birds->getBird(i).xDir;

			if (vx >= 0.1f)
				birdsheet.getSprite(1, 1).draw(g_renderer, { x, y }, 0.5f, SDL_FLIP_NONE);
			else if (vx <= 0.1f)
				birdsheet.getSprite(1, 1).draw(g_renderer, { x, y }, 0.5f, SDL_FLIP_HORIZONTAL);
		}
	}

	// present
	SDL_RenderPresent(g_renderer);
}

void animate(float dt)
{}

void sound()
{
	if (not Mix_PlayingMusic())
		Mix_PlayMusic(g_ambience, -1); //(-1 = "loop forever")
}


void loadContent()
{
	g_texture = IMG_LoadTexture(g_renderer, "content/spritesheet.png"); // NEW
	g_sheepTexture = IMG_LoadTexture(g_renderer, "content/sheepsheet.png"); // NEW
	g_wolfTexture = IMG_LoadTexture(g_renderer, "content/wolfsheet.png"); // NEW
	g_birdTexture = IMG_LoadTexture(g_renderer, "content/birdsheet.png"); // NEW
	g_backgroundTexture = IMG_LoadTexture(g_renderer, "content/hill.png"); // NEW
	g_ambience = Mix_LoadMUS("content/nature.wav"); // NEW
	if (g_texture == NULL)
		assert(false && "Texture loading failed!");
	else if (g_backgroundTexture == NULL)
		assert(false && "Background texture loading failed!");
	else if (g_sheepTexture == NULL)
		assert(false && "Sheep texture loading failed!");
	else if (g_wolfTexture == NULL)
		assert(false && "Wolf texture loading failed!");
	else if (g_ambience == NULL)
		assert(false && "Ambience loading failed!");
}

void shutdown()
{
	Mix_FreeMusic(g_ambience);
	g_ambience = nullptr;
	SDL_DestroyTexture(g_texture);
	g_texture = nullptr;
	SDL_DestroyTexture(g_sheepTexture);
	g_sheepTexture = nullptr;
	SDL_DestroyTexture(g_wolfTexture);
	g_wolfTexture = nullptr;
	SDL_DestroyTexture(g_birdTexture);
	g_birdTexture = nullptr;
	SDL_DestroyTexture(g_backgroundTexture);
	g_backgroundTexture = nullptr;
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) // NEW
		assert(false && "SDL initialization failed!");
	else {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) // NEW
			assert(false && "SDL_mixer initialization failed!");
		else {
			if (IMG_Init(IMG_INIT_PNG) == NULL) // NEW
				assert(false && "SDL_image initialization failed!");
			else {
				g_window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0); // NEW
				if (g_window == NULL)
					assert(false && "window creation failed!");
				else {
					g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED); // NEW
					if (g_renderer == NULL)
						assert(false && "renderer creation failed!");
				}
			}
		}
	}
}

// MAIN
int main(int argv, char* argc[])
{
	initialize();
	loadContent();
	
	srand(time(NULL));

	g_world = new World(20, 20, 20); // NEW

	float oldTime = SDL_GetTicks() * 0.001f;

	while (pollEvents() == true)
	{
		const float currentTime = SDL_GetTicks() * 0.001;
		const float deltaTime = currentTime - oldTime;
		oldTime = currentTime;
		
		g_world->update(deltaTime * timeScale);
		
		animate(deltaTime);
		display();
		sound();

		SDL_Delay(10);
	}

	delete g_world;
	g_world = nullptr;

	shutdown();
	return 0;
}