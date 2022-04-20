#pragma once
//#include <SDL.h>
#include "Include/SDL2-2.0.14/include/SDL.h"
#include <string>
#include "Direction.h"
#include "Renderer.h"


//The code is taken from https://lazyfoo.net/tutorials/SDL/index.php

// Texture wrapper class
class LTexture
{
private:
	static SDL_Renderer* gRenderer;
	//The actual hardware texture
	SDL_Texture* m_texture;
	void* m_pixels;
	int m_pitch;

	//Image dimensions
	int m_width;
	int m_height;

	//Pixel manipulators
	bool lockTexture();
	bool unlockTexture();

	//bool dig(SDL_Rect position, Direction direction);

	bool dig(const SDL_Rect& position);

	bool loadStreamableFromFile(std::string path);

	friend class Map;
public:
	static void setRenderer(SDL_Renderer* renderer);
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int paddingY = 60);

	//Gets image dimensions
	int getWidth();
	int getHeight();

};