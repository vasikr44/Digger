#include "Gold.h"


const SDL_Rect Gold::COIN_IMAGE = { 3 * Cell::CELL_SIZE, 3 * Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE};

int Gold::coinsTicks = 4000;

int Gold::delayBeforeFalling = 1500;

Gold::Gold(SDL_Point position) : MovingSprite(500, position), m_isCoins(false), falling_blocks(0), m_isFalling(false)
{
    m_velocity = 5;
	m_coinsAnimation.addFrame(COIN_IMAGE);
}

bool Gold::isCoins()
{
	return m_isCoins;
}

bool Gold::tryToMove(const Direction& direction)
{
    if(m_isFalling && direction != DOWN)
    {
        return false;
    }

    if (direction != UP && currCell.tryToMove(direction))
    {
        if (direction == DOWN && isWall(direction))
        {
            if (falling_blocks < 2)
            {
                backToBasicState();
            }
            else
            {
                backToBasicState();
                m_isCoins = true;
                timeAsCoins.start();
            }
            return false;
        }

        nextCell = currCell;
        nextCell.move(direction);
        return true;
    }
    else if (direction == DOWN)
    {
        if(falling_blocks < 2)
        {
            backToBasicState();
        }
        else
        {
            backToBasicState();
            m_isCoins = true;
            timeAsCoins.start();
        }
        return false;
    }


    return false;
}

void Gold::render()
{
    updateState();

    if (m_isCoins)
    {
        m_coinsAnimation.render(m_pos.x, m_pos.y);
    }
    else {
        updatePosition();
        GameObject::render();
    }
}


void Gold::backToBasicState()
{
     resetMovingInfo(STILL);
    falling_blocks = 0;
    m_isFalling = false;
    timeAsCoins.stop();
    delayForFalling.stop();
}

void Gold::updateState()
{
    if (!m_isFalling && falling_blocks > 1)
    {
        backToBasicState();
        m_isCoins = true;
        timeAsCoins.start();
        falling_blocks = 0;
    }
    else if (!m_isFalling && delayForFalling.getTicks() > delayBeforeFalling)
    {
        m_isFalling = true;
        fall();
    }
    else if (m_isCoins && timeAsCoins.getTicks() > coinsTicks)
    {
        erase();
    }
    else if (!m_isFalling && falling_blocks > 0 && !m_isCoins)
    {
        backToBasicState();
    }
}

void Gold::checkForEnptyCell()
{
    if (!delayForFalling.isStarted() && !isWall(DOWN))
    {
        m_isFalling = true;
        fall();

        if (!delayForFalling.isStarted())
        {
            delayForFalling.start();
        }
    }
}


void Gold::fall()
{
    move(DOWN);
}


bool Gold::handleFallingCollision(MovingSprite& sprite, Digger& player)
{
    if (!m_isFalling || (!(currCell == sprite.getCurrentCell()) && !isUnder(sprite)))
    {
        return false;
    }

    if (&sprite != &player)
    {
        player.addPoints(sprite.getPoints());
    }
    sprite.erase();
    return true;
}

bool Gold::handleCoinsCollision(MovingSprite& sprite, Digger& player)
{
    if (!m_isCoins || !(currCell == sprite.getCurrentCell()))
    {
        return false;
    }
    
    if (&sprite == &player)
    {
        player.addPoints(m_points);
    }
    erase();
    return true;
}

bool Gold::isUnder(MovingSprite& sprite)
{
    Cell temp = sprite.getCurrentCell();
    return (temp.move(UP) && temp == currCell && (sprite.getX() >= m_pos.x && sprite.getX() <= m_pos.x + Cell::CELL_SIZE
        || sprite.getX() + Cell::CELL_SIZE >= m_pos.x && sprite.getX() + Cell::CELL_SIZE <= m_pos.x + Cell::CELL_SIZE));
}

bool Gold::checkUnder(MovingSprite& sprite)
{
    if (isUnder(sprite))
    {
        if (sprite.getMoveDirection() == UP && sprite.isMoving())
        {
            sprite.resetMovingInfo(RIGHT);
        }

        delayForFalling.start();
        return true;
    }
    return false;
}


bool Gold::checkHorizontal(MovingSprite& sprite)
{
    if (currCell.isHorizontalNeighbour(sprite.getCurrentCell()))
    {
        if (sprite.getMoveDirection() == RIGHT && sprite.getX() < m_pos.x)
        {
            move(RIGHT);
            sprite.move(LEFT);
            sprite.stopMoving();
        }
        else if (sprite.getMoveDirection() == LEFT && sprite.getX() > m_pos.x)
        {
            move(LEFT);
            sprite.move(RIGHT);
            sprite.stopMoving();
        }
        return true;
    }
    return false;
}

bool Gold::handleBasicStateCollision(MovingSprite& sprite)
{
    if (m_isCoins || m_isFalling)
    {
        return false;
    }
    return checkUnder(sprite) || checkHorizontal(sprite);
}

void Gold::checkCollisionWithSprites(vector<Hobbin>& enemies, Digger& player)
{
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (handleFallingCollision(enemies[i], player) ||
            handleCoinsCollision(enemies[i], player) ||
            handleBasicStateCollision(enemies[i])) {
            continue;
        }
    }

    if (handleFallingCollision(player, player) ||
        handleCoinsCollision(player, player) ||
        handleBasicStateCollision(player)) {
        return;
    }

}


void Gold::updateDirection()
{
    if (nextCell == currCell && m_nextDir!= UP) {
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

void Gold::updatePosition()
{
    if (!moving && (m_moveDir == m_nextDir || m_moveDir == STILL))
    {
        return;
    }
    else {
        updateDirection();

        switch (m_moveDir)
        {
        case DOWN:
            m_pos.y = std::min(m_pos.y + m_velocity, nextCell.y());
            break;
        case LEFT:
            m_pos.x = std::max(m_pos.x - m_velocity, nextCell.x());
            moving = false;
            break;
        case RIGHT:
            m_pos.x = std::min(m_pos.x + m_velocity, nextCell.x());
            moving = false;
            break;
        }

        if ((m_moveDir == DOWN  && m_pos.y == nextCell.y())
            || (m_moveDir == LEFT || m_moveDir == RIGHT) && m_pos.x == nextCell.x())
        {
            currCell = nextCell;
            if (m_moveDir == DOWN)
            {
                falling_blocks++;
            }
            else {
                checkForEnptyCell();
            }
        }
    }
}

void Gold::reset()
{
    if (m_isCoins) 
    {
        erase();
    }
    else
    {
        m_isFalling = false;
        delayForFalling.stop();
        checkForEnptyCell();
    }
}