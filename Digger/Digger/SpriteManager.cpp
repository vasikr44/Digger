#include <cstdlib>
#include <iostream>
#include "Cell.h"
#include "Direction.h"
#include "Renderer.h"
#include "SpriteManager.h"

const int SpriteManager::MAX_LIVES = 3;
const int SpriteManager::MAX_ENEMIES_COUNT = 5;
const int SpriteManager::NEW_ENEMY_TICKS = 2000;
const int SpriteManager::BONUS_MODE_TICKS = 4000;

const SDL_Rect SpriteManager::GOLD_IMAGE = { 180, 60, 60, 60 };
const SDL_Rect SpriteManager::EMERALD_IMAGE = { 180, 0, 60, 60 };
const SDL_Rect SpriteManager::CHERRY_IMAGE = { 180, 120, 60, 60 };

const SDL_Point SpriteManager::ENEMY_SPAWN_POSITION = { (Map::MAP_WIDTH - 1) * Cell::CELL_SIZE, 0 };
const SDL_Point SpriteManager::DIGGER_SPAWN_POSITION = { 420, 540 };
const SDL_Color SpriteManager::TEXT_COLOR = { 255, 165, 0 };

SpriteManager::SpriteManager()
    : m_player(DIGGER_SPAWN_POSITION), m_cherry(1000, ENEMY_SPAWN_POSITION), m_map(nullptr), m_isBonusMode(false), 
      m_showCherry(false) ,m_enemiesCnt(MAX_ENEMIES_COUNT), m_newEnemyTicks(NEW_ENEMY_TICKS)
{
    for (int i = 0; i < 3; i++)
    {
        m_player.addFrame({ 0, i * Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE });
    }

    m_player.addFrame({ 0, Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE });
    livesPlayer.addFrame({ 0, Cell::CELL_SIZE, Cell::CELL_SIZE, Cell::CELL_SIZE });

    m_cherry.addFrame(CHERRY_IMAGE);

    createEnemy();
    m_nextEnemy.start();
}


void SpriteManager::setMap(Map* map)
{
    m_map = map;
    m_player.setMap(map);
    
    for (int i = 0; i < m_enemies.size(); i++)
    {
        m_enemies[i].setMap(map);
    }
}


void SpriteManager::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: m_player.move(UP); break;
        case SDLK_DOWN: m_player.move(DOWN); break;
        case SDLK_LEFT: m_player.move(LEFT); break;
        case SDLK_RIGHT: m_player.move(RIGHT); break;
        case SDLK_SPACE: m_player.setCharging(true);
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        m_player.stopMoving();
    }
}

void SpriteManager::render()
{
    if (m_enemiesCnt <= 0 && m_player.isAlive() && !m_bonusModeTimer.isPaused() && rand() % 10 > 3)
    {
        m_showCherry = true;
    }

    checkForCollision();
    deleteSprites();

    if (!m_player.isAlive() && !m_player.shouldBeDeleted())
    {
        clear();
    }
    createEnemy();

    renderFrame();
}

void SpriteManager::checkForCollision()
{
    for (vector<Gold>::iterator i = m_gold.begin(); i != m_gold.end(); ++i)
    {
        i->checkCollisionWithSprites(m_enemies, m_player);
    }

    m_player.checkCollisionWithHobbin(m_enemies, m_isBonusMode);

    for (vector<Hobbin>::iterator i = m_enemies.begin(); i != m_enemies.end(); ++i)
    {
        if (!i->shouldBeDeleted())
        {
            i->checkCollisionWithObjects(m_emeralds);
            i->checkCollisionWithHobbin(m_enemies);
        }
        if (i->isNobbin() && rand() % 10 > 2) {
            i->setDestination(m_player.getX(), m_player.getY());
        }
    }
    checkCherryCollision();
    if (m_player.isAlive())
    {
        m_player.checkCollisionWithObjects(m_emeralds);
    }
}

void SpriteManager::addObject(size_t index, const SDL_Point& position)
{

    switch (index)
    {
    case GOLD:
        m_gold.push_back(Gold(position));
        m_gold[m_gold.size() - 1].addFrame(GOLD_IMAGE);
        m_gold[m_gold.size() - 1].setMap(m_map);
        break;
    case EMERALD:
        m_emeralds.push_back(GameObject(25, position));
        m_emeralds[m_emeralds.size() - 1].addFrame(EMERALD_IMAGE);
        break;
    case CHERRY:
        m_emeralds.push_back(GameObject(1000, position));
        m_emeralds[m_emeralds.size() - 1].addFrame(CHERRY_IMAGE);
        break;
    }
}

void SpriteManager::createEnemy()
{
    if (m_enemiesCnt <= 0 || m_nextEnemy.getTicks() < m_newEnemyTicks)
    {
        return;
    }

    m_nextEnemy.start();
    --m_enemiesCnt;
    m_enemies.push_back(Hobbin(ENEMY_SPAWN_POSITION));
    m_newEnemyTicks += NEW_ENEMY_TICKS;

    for (int i = 0; i < 4; i++)
    {
        m_enemies[m_enemies.size() - 1].addFrame({ Cell::CELL_SIZE*2, Cell::CELL_SIZE * i, Cell::CELL_SIZE, Cell::CELL_SIZE });
    }
    m_enemies[m_enemies.size() - 1].setMap(m_map);
    m_enemies[m_enemies.size() - 1].setDestination(m_player.getX(), m_player.getY());
}


void SpriteManager::deleteSprites()
{
    //to be optimized!
    for (vector<GameObject>::iterator i = m_emeralds.begin(); i != m_emeralds.end(); )
    {
        if (i->shouldBeDeleted()) {
            i = m_emeralds.erase(i);
        }
        else
            ++i;
    }

    for (vector<Hobbin>::iterator i = m_enemies.begin(); i != m_enemies.end(); )
    {
        if (i->shouldBeDeleted()) {
            i = m_enemies.erase(i);
        }
        else
        {
            if (rand() % 10 + 1 > 5)
            {
                i->setDestination(m_player.getX(), m_player.getY());
            }
            ++i;
        }
    }

    for (vector<Gold>::iterator i = m_gold.begin(); i != m_gold.end(); )
    {
        if (i->shouldBeDeleted()) {
            i = m_gold.erase(i);
        }
        else {
            ++i;
        }
    }
}

void SpriteManager::clear()
{
    m_showCherry = false;
    m_cherry.m_delete = false;
    m_isBonusMode = false;

    m_enemiesCnt = MAX_ENEMIES_COUNT + m_map->getLevel();
    m_newEnemyTicks = NEW_ENEMY_TICKS;
    m_enemies.clear();

    for (vector<Gold>::iterator i = m_gold.begin(); i != m_gold.end(); ++i)
    {
        i->reset();
        if (i->isCoins())
        {
            i->erase();
        }
    }
    m_player.reset(DIGGER_SPAWN_POSITION);
}

void SpriteManager::reset()
{
    clear();
    m_enemies.clear();
    m_gold.clear();
    m_emeralds.clear();
}


bool SpriteManager::nextLevel()
{
    return m_player.isAlive() && ((m_enemiesCnt <= 0 && m_enemies.size() == 0) || m_emeralds.size() == 0);
}

bool SpriteManager::isGameOver()
{
    return m_player.shouldBeDeleted();
}


void SpriteManager::renderFrame()
{
    for (vector<GameObject>::iterator i = m_emeralds.begin(); i != m_emeralds.end(); ++i)
    {
        i->render();
    }
    for (vector<Hobbin>::iterator i = m_enemies.begin(); i != m_enemies.end(); ++i)
    {
        i->render();
    }
    for (vector<Gold>::iterator i = m_gold.begin(); i != m_gold.end(); ++i)
    {
        i->render();
    }

    if (m_showCherry && !m_cherry.shouldBeDeleted())
    {
        m_cherry.render();
    }

    m_player.render();

    score.loadFromRenderedText(std::to_string(m_player.getPoints()), TEXT_COLOR);
    score.render(0, 0, 0, 0.0, 0, SDL_FLIP_NONE, 0);

    for (int i = 0; i < m_player.getLivesLeft(); i++)
    {
        livesPlayer.render(Cell::CELL_SIZE * i + score.getWidth(), 0, 0);
    }

    if (m_isBonusMode)
    {
        score.loadFromRenderedText("BONUS MODE", TEXT_COLOR);
        score.render(Renderer::ScreenWidth() / 2 - 90, 0, 0, 0.0, 0, SDL_FLIP_NONE, 0);
    }

    
}


void SpriteManager::checkCherryCollision()
{
    if (!m_showCherry)
    {
        return;
    }

    if (!m_cherry.shouldBeDeleted() && m_player.checkCollision(m_cherry))
    {
        m_isBonusMode = true;
        m_bonusModeTimer.start();
    }
    else if (m_bonusModeTimer.getTicks() > BONUS_MODE_TICKS)
    {
        m_cherry.deleteObject();
        m_bonusModeTimer.stop();
        m_isBonusMode = false;
        m_showCherry = false;
    }
}