#include "Hobbin.h"
#include "Cell.h"

const int Hobbin::MAX_TIME_AS_NOBBIN = 8000;
int Hobbin::requiredCrossesForNobbin = 600;
int Hobbin::timesEnemiesCrossed = 0;

Hobbin::Hobbin(SDL_Rect& imageClip, SDL_Point position)
    :MovingSprite(imageClip, 250, position), destination(0, 0), lastDestination(0, 1), nobbin(false)
{
    for (int i = 0; i < 4; i++)
    {
        nobbinAnimation.addFrame({ Cell::CELL_SIZE, i * Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE });
    }
}

Hobbin::Hobbin(SDL_Point position)
    : MovingSprite(250, position), destination(0, 0), lastDestination(0, 1), nobbin(false)
{
    for (int i = 0; i < 4; i++)
    {
        nobbinAnimation.addFrame({ Cell::CELL_SIZE, i * Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE });
    }
}

void Hobbin::setDirectionFromPathfinder()
{
    Pair p = pathfinder.final_path.top();
    pathfinder.final_path.pop();

    Pair pos(m_pos.x / Cell::CELL_SIZE, m_pos.y / Cell::CELL_SIZE);

    if (pos == p && !pathfinder.final_path.empty()) {
        p = pathfinder.final_path.top();
        pathfinder.final_path.pop();
    }
    if (p.first < pos.first && currCell.tryToMove(LEFT))
    {
        if (nobbin || !isWall(LEFT))
        {
            move(LEFT);
        }
    }
    else if (p.first > pos.first && currCell.tryToMove(RIGHT))
    {
        if (nobbin || !isWall(RIGHT))
        {
            move(RIGHT);
        }
    }
    else if (p.second > pos.second && currCell.tryToMove(DOWN))
    {
        if (nobbin || !isWall(DOWN))
        {
            move(DOWN);
        }
    }
    else if (p.second < pos.second && currCell.tryToMove(UP))
    {
        if (nobbin || !isWall(UP))
        {
            move(UP);
        }
    }
    else 
    {
        setMoveDirection();
    }
    findPath();
}

void Hobbin::render()
{
    if (currCell == nextCell && pathfinder.final_path.size() > 0)
    {
        setDirectionFromPathfinder();
    }
    else if (pathfinder.final_path.empty())
    {
        findPath();
    }

    updatePosition();

    if (!nobbin)
    {
        if (timesEnemiesCrossed > requiredCrossesForNobbin)
        {
            timesEnemiesCrossed = 0;
            requiredCrossesForNobbin += 200;
            nobbin = true;
            findPath();
            timeAsNobbin.start();
        }
        GameObject::render();
    }
    else
    {
        m_map->dig({ m_pos.x, m_pos.y, Cell::CELL_SIZE, Cell::CELL_SIZE });
        nobbinAnimation.render(m_pos.x, m_pos.y);
        if (timeAsNobbin.getTicks() > MAX_TIME_AS_NOBBIN)
        {
            nobbin = false;
        }
    }
}


void Hobbin::findPath()
{
    if (m_map == NULL || lastDestination == destination)
    {
        return;
    }

    while (!pathfinder.final_path.empty()) { pathfinder.final_path.pop(); }

    for (size_t y = 0; y < Map::MAP_HEIGHT; y++)
    {
        for (size_t x = 0; x < Map::MAP_WIDTH; x++)
        {
            if (nobbin || m_map->getValue(x, y) == '0')
            {
                pathfinder.setValue(x, y, 1);
            }
            else 
            {
                pathfinder.setValue(x, y, 0);
            }
        }
    }
    // Source is the left-most bottom-most corner
    Pair src(m_pos.x / Cell::CELL_SIZE, m_pos.y / Cell::CELL_SIZE);

    
    // Destination is the left-most top-most corner
    Pair dest(destination.first, destination.second);
    pathfinder.aStarSearch(src, dest);
}


void Hobbin::setMoveDirection()
{
    Cell c = currCell;
    if (c.tryToMove(LEFT) && isWall(LEFT))
    {
        move(LEFT);
    }
    else if (c.tryToMove(DOWN) && isWall(DOWN))
    {
        move(DOWN);
    }
    else if (c.tryToMove(UP) && isWall(UP))
    {
        move(UP);
    }
    else if (c.tryToMove(RIGHT) && isWall(RIGHT))
    {
        move(RIGHT);
    }
}


void Hobbin::setDestination(size_t x, size_t y)
{
    lastDestination = destination;
    moving = true;

    destination.first = x / Cell::CELL_SIZE;
    destination.second = y / Cell::CELL_SIZE;
    if (nobbin)
    {
        findPath();
    }
}


bool Hobbin::reachedGoal()
{
    return abs(m_pos.x / Cell::CELL_SIZE - destination.first) +
        abs(m_pos.y / Cell::CELL_SIZE - destination.second) <= 2;
}


bool Hobbin::isNobbin()
{
    return nobbin;
}


void Hobbin::checkCollisionWithObjects(vector<GameObject>& objects)
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        checkCollision(objects[i]);
    }
}

bool Hobbin::checkCollision(GameObject& object)
{
    if (currCell ==  object.getPosition() && !object.shouldBeDeleted())
    {
        object.deleteObject();
        return true;
    }
    return false;
}

void Hobbin::checkCollisionWithHobbin(vector<Hobbin>& hobbins)
{
    for (size_t i = 0; i < hobbins.size(); i++)
    {
        if (!hobbins[i].shouldBeDeleted() && &hobbins[i] != this && currCell == hobbins[i].getCurrentCell())
        {
            if (destination == hobbins[i].destination)
            {
                setDestination(0, 0);
                hobbins[i].setDestination(14, 9);
            }
            ++timesEnemiesCrossed;
        }
    }
}