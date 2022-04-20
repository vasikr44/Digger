#include "Game.h"
#include "Map.h"
#include <iostream>

Game::Game() : instance(nullptr)
{
	Renderer::setScreenSize(900, 660, 60);

	try {
		//~TODO~ remove reference
		instance = &Renderer::getInstance();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << e.what();
		return;
	}
	catch (const std::exception& e)
	{
		return;
	}
}
Game::~Game()
{}

void Game::start()
{
	Animation::loadTexture("Resources/sprites.png");

	map.loadNextMap();
	sprites.setMap(&map);

	map.loadObjects(sprites);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;
	sprites.render();

	//While application is running
	while (!quit)
	{
		//Handle events on queue 
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else
			{
				sprites.handleEvent(e);
			}
		}


		instance->clearScreen();
		/*background.dig(sprites.diggerPosition(), UP);
		background.render(0, 0);*/
		map.render();
		sprites.render();

		instance->renderCurrentFrame();

		if (sprites.nextLevel())
		{
			sprites.reset();
			map.loadNextMap();
			map.loadObjects(sprites);
		}
		else if (sprites.isGameOver())
		{
			quit = true;;
		}
	}
	quit = false;
	while (!quit)
	{
		instance->clearScreen();
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		gameOver.loadFromRenderedText("GAME OVER", { 255,165,0 });
		gameOver.render(300, 300,0,0.0, 0, SDL_FLIP_NONE, 0);
		instance->renderCurrentFrame();
	}
}