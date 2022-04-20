#include "MovingSprite.h"
#include "Renderer.h"
#include "Direction.h"
#include "Cell.h"
#include <cmath>
#include <iostream>

MovingSprite::MovingSprite(const SDL_Rect& imageClip, int points, SDL_Point position)
		 : GameObject(imageClip, points, position), m_velocity(3), m_moveDir(RIGHT), m_nextDir(RIGHT),
           moving(false), currCell(position), nextCell(position), m_map(nullptr)
{}

MovingSprite::MovingSprite(int points, SDL_Point position)
         : GameObject(points, position), m_velocity(3), m_moveDir(RIGHT), m_nextDir(RIGHT), 
           moving(false), currCell(position), nextCell(position), m_map(nullptr)
{}

void MovingSprite::setMap(Map* map)
{
    m_map = map;
}

void MovingSprite::setVelocity(unsigned int velocity)
{
    m_velocity = velocity;
}

void MovingSprite::render()
{
    updatePosition();
    GameObject::render();
}

int MovingSprite::getPoints()
{
    return m_points;
}

void MovingSprite::move(const Direction& dir)
{
    moving = true;
    m_nextDir = dir;
}

bool MovingSprite::tryToMove(const Direction& direction)
{
    if (currCell.tryToMove(direction) && !isWall(direction))
    {
        nextCell = currCell;
        nextCell.move(direction);
        return true;
    }

    return false;
}

bool MovingSprite::isWall(const Direction& direction)
{
    Cell temp = currCell;
    temp.move(direction);
    
    return (m_map->getValue(temp.x() / Cell::CELL_SIZE, temp.y() / Cell::CELL_SIZE) >= '1');
}

void MovingSprite::updateDirection()
{
    if (nextCell == currCell) {
        if (m_nextDir != m_moveDir && tryToMove(m_nextDir))
            m_moveDir = m_nextDir;
        else
        {
            if (!tryToMove(m_moveDir))
            {
                moving = false;
                return;
            }
        }

    }
    else if (m_moveDir == STILL)
    {
        if (tryToMove(m_nextDir))
        {
            m_moveDir = m_nextDir;
        }
    }
}

void MovingSprite::updatePosition()
{
    if (!moving)
    {
        return;
    }
    else {
        updateDirection();

        switch (m_moveDir)
        {
        case UP:
            m_pos.y = std::max(m_pos.y - m_velocity, nextCell.y());
            break;
        case DOWN:
            m_pos.y = std::min(m_pos.y + m_velocity, nextCell.y());
            break;
        case LEFT:
            m_pos.x = std::max(m_pos.x - m_velocity, nextCell.x());
            break;
        case RIGHT:
            m_pos.x = std::min(m_pos.x + m_velocity, nextCell.x());
            break;
        }

        if (((m_moveDir == DOWN || m_moveDir == UP) && m_pos.y == nextCell.y())
            || (m_moveDir == LEFT || m_moveDir == RIGHT) && m_pos.x == nextCell.x())
        {
            currCell = nextCell;
        }
    }
}

void MovingSprite::stopMoving()
{
    moving = false;
}


const Direction& MovingSprite::getMoveDirection() const
{
    return m_moveDir;
}

const Cell& MovingSprite::getCurrentCell()
{
    return currCell;
}


void MovingSprite::erase()
{
    m_delete = true;
}

bool MovingSprite::isMoving()
{
    return moving;
}

void MovingSprite::resetMovingInfo(const Direction& direction)
{
    m_pos.x = currCell.x();
    m_pos.y = currCell.y();
    m_moveDir = direction;
    m_nextDir = direction;
    moving = false;
    nextCell = currCell;
}