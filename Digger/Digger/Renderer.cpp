#include <iostream>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "Animation.h"
#include "Renderer.h"

int Renderer::SCREEN_WIDTH = 900;
int Renderer::SCREEN_HEIGHT = 600;
int Renderer::PADDING_Y = 0;
TTF_Font* Renderer::gFont = NULL;

Renderer::Renderer()
{
	initVideo();

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	initWindow();
	initRenderer();
	initText();
	//loadMedia();
}


Renderer& Renderer::getInstance() {
	static Renderer instance;
	return instance;
}

void Renderer::initText()
{
	if (TTF_Init() == -1)
	{
		throw std::runtime_error("SDL_ttf could not initialize!\n");
	}

	gFont = TTF_OpenFont("Resources/ARCADECLASSIC.TTF", 56);
	if (gFont == NULL)
	{
		throw std::runtime_error("Font couldn't be loaded!\n");
	}
}


void Renderer::initVideo()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error(SDL_GetError());
	}
}

void Renderer::initWindow()
{
	gWindow = SDL_CreateWindow("Digger", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		throw std::runtime_error(SDL_GetError());
	}
}

void Renderer::initRenderer()
{
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		throw std::runtime_error(SDL_GetError());
	}


	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		throw std::runtime_error(SDL_GetError());
	}

	LTexture::setRenderer(gRenderer);
}


void Renderer::close()
{
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

Renderer::~Renderer()
{
	close();
}

void Renderer::setScreenSize(int width, int height, int padding_y)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	PADDING_Y = padding_y;
}

void Renderer::clearScreen()
{
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);
}

void Renderer::renderCurrentFrame()
{
	//Update screen
	SDL_RenderPresent(gRenderer);
}


TTF_Font* Renderer::font()
{
	return gFont;
}

int Renderer::ScreenWidth()
{
	return SCREEN_WIDTH;
}

int Renderer::ScreenHeight()
{
	return SCREEN_HEIGHT;
}

int Renderer::PaddingY()
{
	return PADDING_Y;
}


Uint32 Renderer::getWingowFormat()
{
	return SDL_GetWindowPixelFormat(gWindow);
}