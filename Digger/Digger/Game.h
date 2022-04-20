#pragma once
#include "Map.h"
#include "Texture.h"
#include "Renderer.h"
#include "SpriteManager.h"

class Game
{
private:
	/**
	 * @brief Holds an instance to the renderer
	 * Used pointer to be able to catch the error if the renderer didn't load correctly
	*/
	Renderer* instance;
	/**
	 * @brief Holds a texture for the game over text when the player loses all its lives
	*/
	LTexture gameOver;
	/**
	 * @brief SpriteManager object managing all of the sprites
	*/
	SpriteManager sprites;
	/**
	 * @brief Holds a map object holding the game map and storing the background
	*/
	Map map;

public:
	/**
	 * @brief Default constructor
	*/
	Game();
	~Game();

	/**
	 * @brief function called to initialize everything and implement the game loop
	*/
	void start();
};

