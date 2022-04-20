#include "Bullet.h"
#include "Hobbin.h"
#include "Digger.h"

const SDL_Rect Bullet::ANIMATION_FRAME_1 = { 2 * Cell::CELL_SIZE, 4 * Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE };
const SDL_Rect Bullet::ANIMATION_FRAME_2 = { 3 * Cell::CELL_SIZE, 4 * Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE };

Bullet::Bullet(const Cell& cell, const Direction& direction) 
	: MovingSprite(0, { cell.x(), cell.y() })
{
	m_animation.addFrame(ANIMATION_FRAME_1);
	m_animation.addFrame(ANIMATION_FRAME_2);

	m_moveDir = direction;
	m_nextDir = direction;

	m_velocity = 5;
	moving = true;
}

Bullet::Bullet(const Cell& cell) : MovingSprite(0, { cell.x(), cell.y() })
{
	m_animation.addFrame(ANIMATION_FRAME_1);
	m_animation.addFrame(ANIMATION_FRAME_2);
}

void Bullet::move(const Direction& dir)
{
	if (!moving)
	{
		move(dir);
	}
}

bool Bullet::tryToMove(const Direction& direction)
{
	if (!MovingSprite::tryToMove(direction))
	{
		moving = false;
		erase();
		return false;
	}

	return true;
}


void Bullet::checkCollisionWithEnemy(vector<Hobbin>& enemies, Digger& player)
{
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (moving && currCell == enemies[i].getCurrentCell() && !enemies[i].shouldBeDeleted())
		{
			player.addPoints(enemies[i].getPoints());
			enemies[i].erase();
			moving = false;
		}
	}
}

void Bullet::render()
{
	if (moving)
	{
		updatePosition();
		GameObject::render();
	}
}


void Bullet::reset(const Cell& cell, const Direction& direction)
{
	setX(cell.x());
	setY(cell.y());
	currCell = cell;
	nextCell = currCell;

	m_moveDir = direction;
	m_nextDir = direction;

	m_velocity = 5;
	moving = true;
}