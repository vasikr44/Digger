#pragma once
#include "Texture.h"

class SpriteManager;

class Map
{
	static const char background_File_Name[]; ///holds the basic background name
	static const char background_File_Type[]; ///holds the background type(png)
	static const char map_File_Name[]; ///holds the basic map name
	static const char map_File_Type[]; ///holds the map file type(txt)

	LTexture background; ///Holds the background
	size_t currentMapIndex; ///Loads the map and background with the current index
	SDL_Point m_digFrom; ///Helper variable to dig the start tunel
	char map[15][10]; /// Holds the info about the field

	void dig(const SDL_Rect& position); /// function for digging the background

	void readChar(std::fstream& file, int x, int y);///reads a char from the given file
	void loadObjects(SpriteManager& manager); ///loads the objects in the sprite manager
	bool loadNextMap(); ///loads the next map
	bool loadBackground(); /// loads the background with the map(Called from loadNextMap)
	void digMap();
	void dig(const SDL_Point& from, const SDL_Point& to);
	void checkForDig(size_t row, size_t col);

	//These classes need to access some of the private functions
	friend class MovingSprite;
	friend class Hobbin;
	friend class Digger;
	friend class Gold;
	friend class Game;

public:
	static const int MAP_WIDTH = 15; /// The width of the field
	static const int MAP_HEIGHT = 10; /// The height of the field

	Map(); /// Default constructor
	
	void render(); ///Renders the background

	/**
	 * @brief returns the value of the cell with coordinates x and y
	*/
	char getValue(size_t x, size_t y);
	/**
	 * @return The current level index
	*/
	size_t getLevel();
};
