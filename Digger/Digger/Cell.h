#pragma once
//#include <SDL.h>
#include "Include/SDL2-2.0.14/include/SDL.h"
#include "Direction.h"
#include "Renderer.h"

//To be deleted?
class Cell
{
private:
	/**
	 * @brief Holds a rectangle representing a cell in a grid.
	 * Therefore the left top corner of the cell should correspond to a top left corner in a hypothetical grid.
	 * Having in mind that the cell is a square with dimensions equal to the size of each of the sprites
	*/
	SDL_Rect cell;
public:
	static const int CELL_SIZE; ///Holds the cell size(in my implementation it's 60)

	/**
	 * @brief default constructor creating a cell with (0,0) starting coordinates
	*/
	Cell();
	/**
	 * @brief Copy constructor creating a cell with the same info as other
	 * @param other - the cell we are copying from
	*/
	Cell(const Cell& other);
	/**
	 * @brief Sets the coordinates to the cell that holds the position
	 * @param position 
	*/
	Cell(SDL_Point position);
	/**
	 * @brief Works in the same way as the previous constructor but takes the x and y separated
	*/
	Cell(unsigned int x, unsigned int y);

	/**
	 * @brief Takes a cell and copies its info in the current object
	 * @param other - the cell we are copying from
	 * @return returns a reference to this object
	*/
	Cell& operator=(const Cell& other);
	/**
	 * @brief Operator == checks if two cells are equal
	 * @return returns true if they are equal and false otherwise
	*/
	bool operator==(const Cell& other);

	/**
	 * @return returns the x value of the cell 
	*/
	int x() const;
	/**
	 * @return returns the y value of the cell
	*/
	int y() const;

	/**
	 * @brief If possible moves the cell to the next(neighbour) one in the given position
	 * @param direction 
	 * @return whether the moving was successful
	*/
	bool move(const Direction& direction);
	/**
	 * @brief Checks whether the cell can be moved in the given direction
	 * @return true if it can be moved and false otherwise
	*/
	bool tryToMove(const Direction& direction);

	/**
	 * @brief Checks whether the cell contains a given point
	 * @return true if the cell contains the point and false otherwise
	*/
	bool contains(SDL_Point& p) const;
	/**
	 * @brief Checks whether the cell is over another one
	 * @return true if it is over one and false otherwise
	*/
	bool isOver(const Cell& other) const;
	/**
	 * @brief Check if the given cell is a horizontal neighbour
	 * @return true if the cell is a neighbour and false otherwise
	*/
	bool isHorizontalNeighbour(const Cell& other) const;
	/**
	 * @brief Sets the position of the cell(Validates it)
	*/
	void setPosition(unsigned int x, unsigned int y);
};

