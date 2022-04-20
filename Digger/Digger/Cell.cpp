#include "Cell.h"
#include "Direction.h"
#include <iostream>


const int Cell::CELL_SIZE = 60;

Cell::Cell(const Cell& other)
{
    cell = other.cell;
}

Cell::Cell()
{
    cell.x = 0;
    cell.y = 0;
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;
}

Cell::Cell(SDL_Point position)
{
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;

    setPosition(position.x, position.y);
}

Cell::Cell(unsigned int x, unsigned int y)
{
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;

    setPosition(x, y);
}


Cell& Cell::operator=(const Cell& other)
{
    cell = other.cell;
    return *this;
}



bool Cell::operator==(const Cell& other)
{
    return (cell.x == other.x() && cell.y == other.y());
}

int Cell::x() const
{
	return cell.x;
}
int Cell::y() const
{
	return cell.y;
}

bool Cell::move(const Direction& direction)
{
    if (!tryToMove(direction))
    {
        return false;
    }



    switch (direction)
    {
    case UP:
        cell.y -= cell.h;
        break;
    case DOWN:
        cell.y += cell.h;
        break;
    case LEFT:
        cell.x -= cell.w;
        break;
    case RIGHT:
        cell.x += cell.w;
        break;
    }

    return true;
}


bool Cell::tryToMove(const Direction& direction)
{
    switch (direction)
    {
    case UP:
        return cell.y - cell.h >= 0;
    case DOWN:
        return cell.y + cell.h < Renderer::ScreenHeight() - Renderer::PaddingY();
    case LEFT:
        return cell.x - cell.w >= 0;
    case RIGHT:
        return cell.x + cell.w < Renderer::ScreenWidth();
    }

    return false;
}

void Cell::setPosition(unsigned int x, unsigned int y)
{
    if (x % CELL_SIZE != 0 || y % CELL_SIZE != 0)
    {
        cell.x = (x / CELL_SIZE) * CELL_SIZE;
        cell.y = (y / CELL_SIZE) * CELL_SIZE;
    }
    else {
        cell.x = x;
        cell.y = y;
    }

    if (cell.x + cell.w > Renderer::ScreenWidth()) {
        cell.x = Renderer::ScreenWidth() - cell.w;
    }

    if (cell.y + CELL_SIZE > Renderer::ScreenHeight()) {
        cell.y = Renderer::ScreenHeight() - cell.h;
    }
}

bool Cell::contains(SDL_Point& p) const
{
    return !(p.x < cell.x || p.x > cell.x + cell.w || p.y < cell.y || p.y > cell.y + cell.y);
}

bool Cell::isHorizontalNeighbour(const Cell& other) const
{
    Cell tmp(cell.x, cell.y);

    if (tmp.move(LEFT) && tmp == other)
    {
        return true;
    }

    tmp.setPosition(cell.x, cell.y);

    return (tmp.move(RIGHT) && tmp == other);
}


bool Cell::isOver(const Cell& other) const
{
    Cell thisCell(cell.x, cell.y), otherCell(other), otherEdge(other.x() + CELL_SIZE, other.y());
    
    return (thisCell.move(DOWN) && (otherCell == thisCell || (otherEdge == thisCell && !(otherEdge == otherCell))));
}