#include "Map.h"
#include "Cell.h"
#include "SpriteManager.h"
#include <iostream>
#include <fstream>
#include <string>

using std::fstream;
using std::string;

const char Map::background_File_Name[] = "background";
const char Map::background_File_Type[] = ".png";
const char Map::map_File_Name[] = "map";
const char Map::map_File_Type[] = ".txt";

Map::Map() : map{}, currentMapIndex(0), m_digFrom({0,0})
{}

void Map::readChar(fstream& file, int x, int y)
{
	char input;
	file >> input;
	if (input == '\n')
	{
		file >> input;
	}
	map[x][y] = input;
}

bool Map::loadNextMap()
{
	if (!loadBackground())
	{
		return false;
	}

	string path = "Resources/";
	path.append(map_File_Name);
	path += std::to_string(currentMapIndex%7) + map_File_Type;

	fstream file(path, std::ios::in);
	if (!file)
	{
		return false;
	}

	file.seekg(0);

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (!file.eof())
			{
				readChar(file, x, y);
			}
			else
			{
				return false;
			}
		}
	}

	digMap();

	++currentMapIndex;
	return true;
}

void Map::digMap()
{
	for (size_t row = 0; row < MAP_WIDTH; row++)
	{
		m_digFrom.x = -1;
		for (size_t col = 0; col < MAP_HEIGHT; col++)
		{
			checkForDig(row, col);
		}
	}

	for (size_t col = 0; col < MAP_HEIGHT; col++)
	{
		m_digFrom.x = -1;
		for (size_t row = 0; row < MAP_WIDTH; row++)
		{
			checkForDig(row, col);
		}
	}
}


void Map::checkForDig(size_t row, size_t col)
{
	if (map[row][col] == '0')
	{
		if (m_digFrom.x >= 0)
		{
			dig(m_digFrom, { (int)row, (int)col });
		}
		m_digFrom = { (int)row, (int)col };
	}
	else
	{
		m_digFrom.x = -1;
	}
}


void Map::dig(const SDL_Point& from, const SDL_Point& to)
{
	for (int y = from.y * Cell::CELL_SIZE; y <= to.y * Cell::CELL_SIZE; y++)
	{
		for (int x = from.x * Cell::CELL_SIZE; x <= to.x * Cell::CELL_SIZE; x++)
		{
			dig({ x,y,Cell::CELL_SIZE, Cell::CELL_SIZE });
		}
	}
}


bool Map::loadBackground()
{
	string path = "Resources/";
	path.append(background_File_Name);
	path += std::to_string(currentMapIndex % 7) + background_File_Type;

	return background.loadStreamableFromFile(path);
}

void Map::render()
{
	background.render(0, 0);
}

void Map::dig(const SDL_Rect& position)
{
	size_t x = position.x / Cell::CELL_SIZE;
	size_t y = position.y / Cell::CELL_SIZE;

	if (x < MAP_WIDTH && y < MAP_HEIGHT)
	{
		map[x][y] = '0';
		background.dig(position);
	}
}


void Map::loadObjects(SpriteManager& manager)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (map[x][y] > '1' && map[x][y] <= '4')
			{
				manager.addObject((size_t)map[x][y] - '0', { x * Cell::CELL_SIZE, y * Cell::CELL_SIZE });
			}
		}
	}
}

char Map::getValue(size_t x, size_t y) 
{
	if (map)
	{
		return map[x][y];
	}
	
	return 0;
}


size_t Map::getLevel()
{
	return currentMapIndex;
}