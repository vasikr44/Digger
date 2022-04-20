#pragma once
//#include <SDL.h>
#include "Include/SDL2-2.0.14/include/SDL.h"
#include "Include/SDL2_ttf-2.0.15/include/SDL_ttf.h"
//#include <SDL_ttf.h>

//The code is taken from https://lazyfoo.net/tutorials/SDL/index.php

class Renderer
{
private:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static int PADDING_Y;

	static TTF_Font* gFont;

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	/*
	//Starts up SDL and creates window
	bool init();

	//Loads media
	void loadMedia();*/

	//Frees media and shuts down SDL
	void close();

	Renderer();
	
	void initText();
	void initVideo();
	void initWindow();
	void initRenderer();

public:
	static const int BORDER_SIZE = 5;

	~Renderer();

	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;

	static Renderer& getInstance();
	static void setScreenSize(int width, int height, int padding_y = 0);

	static TTF_Font* font();
	static int ScreenWidth();
	static int ScreenHeight();
	static int PaddingY();

	Uint32 getWingowFormat();

	void clearScreen();
	void renderCurrentFrame();
};

