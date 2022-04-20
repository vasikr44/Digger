#include "Digger.h"
#include "Direction.h"

const int Digger::POINTS_FOR_EATEN_ENEMY = 200;

Digger::Digger(SDL_Rect& imageClip, SDL_Point position)
    : MovingSprite(imageClip, 0,position), m_bullet(position), m_consecutiveEmeralds(0), 
     m_currentAnimation(&m_animation), m_chargingTime(5000), m_livesLeft(3), m_bonusModeKillCount(0)
{
    setChargingFrames();
}
Digger::Digger(SDL_Point position)
    : MovingSprite(0, position), m_bullet(position), m_consecutiveEmeralds(0), m_currentAnimation(&m_animation),
      m_chargingTime(5000), m_livesLeft(3), m_bonusModeKillCount(0)
{
    setChargingFrames();
}


void Digger::reset(const SDL_Point& position)
{
    m_pos.x = position.x;
    m_pos.y = position.y;

    currCell = position;
    nextCell = currCell;

    m_consecutiveEmeralds = 0;
    m_currentAnimation = &m_animation;
    m_bonusModeKillCount = 0;
    m_delete = false;
}

bool Digger::tryToMove(const Direction& direction)
{
    if (currCell.tryToMove(direction))
    {
        nextCell = currCell;
        nextCell.move(direction);
        return true;
    }

    return false;
}


void Digger::setChargingFrames()
{
    for (size_t i = 0; i < 2; i++)
    {
        m_chargingAnimation.addFrame({ 0, 180, Cell::CELL_SIZE, Cell::CELL_SIZE });
        m_chargingAnimation.addFrame({ 0, 240, Cell::CELL_SIZE, Cell::CELL_SIZE});
    }
}

void Digger::updatePosition()
{
    switch (m_moveDir)
    {
    case UP:
        m_currentAnimation->flip(SDL_FLIP_HORIZONTAL);
        m_currentAnimation->rotate(90);
        break;
    case DOWN:
        m_currentAnimation->flip(SDL_FLIP_HORIZONTAL);
        m_currentAnimation->rotate(-90);
        break;
    case LEFT:
        m_currentAnimation->flip(SDL_FLIP_HORIZONTAL);
        m_currentAnimation->rotate(0);
        break;
    case RIGHT:
        m_currentAnimation->flip(SDL_FLIP_NONE);
        m_currentAnimation->rotate(0);
        break;
    }

    m_map->dig({ m_pos.x, m_pos.y, Cell::CELL_SIZE, Cell::CELL_SIZE });

    MovingSprite::updatePosition();
}


void Digger::addPoints(size_t points)
{
    m_points += points;
}

void Digger::setCharging(bool isCharging)
{
    if (m_currentAnimation != &m_chargingAnimation)
    {
        m_bullet.reset(currCell, m_moveDir);

        m_currentAnimation = &m_chargingAnimation;
        m_charging.start();
    }
}

void Digger::render()
{
    if (m_charging.getTicks() >= m_chargingTime)
    {
        m_charging.stop();
        m_chargingTime += 200;
        m_currentAnimation = &m_animation;
    }

    m_bullet.render();

    if (m_currentAnimation == &m_chargingAnimation)
    {
        updatePosition();
        m_chargingAnimation.render(m_pos.x, m_pos.y);
    }
    else {
        MovingSprite::render();
    }

}

bool Digger::isCharging()
{
    return (m_currentAnimation == &m_chargingAnimation);
}

bool Digger::isMoving()
{
    return moving;
}


void Digger::checkCollisionWithHobbin(vector<Hobbin>& enemies, bool isBonusMode)
{
    m_bullet.checkCollisionWithEnemy(enemies, *this);
    if (m_delete)
    {
        return;
    }

    for (vector<Hobbin>::iterator i = enemies.begin(); i != enemies.end();++i)
    {
        if (currCell == i->getCurrentCell() && isAlive())
        {
            if (!isBonusMode)
            {
                erase();

                return;
            }
            else
            {
                addPoints(POINTS_FOR_EATEN_ENEMY * (++m_bonusModeKillCount));
                i->erase();
            }
        }
    }
}


void Digger::checkCollisionWithObjects(vector<GameObject>& objects)
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        checkCollision(objects[i]);
    }
}

bool Digger::checkCollision(GameObject& object)
{
    if (currCell == object.getPosition() && !object.shouldBeDeleted())
    {
        addPoints(object.getPoints());
        object.deleteObject();
        return true;
    }

    return false;
}

bool Digger::isAlive()
{
    return !m_delete;
}


bool Digger::shouldBeDeleted()
{
    return(m_delete && m_livesLeft <= 0);
}


void Digger::erase()
{
    if (isAlive())
    {
        m_delete = true;
        --m_livesLeft;

        resetMovingInfo(RIGHT);
    }
}

void Digger::setMap(Map* map)
{
    MovingSprite::setMap(map);
    m_bullet.setMap(map);
}


int Digger::getLivesLeft()
{
    return m_livesLeft;
}