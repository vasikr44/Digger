#pragma once
//#include <SDL.h>
#include "Include/SDL2-2.0.14/include/SDL.h"
#include <vector>
#include "GameObject.h"
#include "Gold.h"
#include "MovingSprite.h"
#include "Digger.h"
#include "Texture.h"
#include "Map.h";
#include "Hobbin.h"

using std::vector;

class SpriteManager
{
private:
    static const int MAX_ENEMIES_COUNT; ///The max count of the enemies in a level
    static const int NEW_ENEMY_TICKS; ///Default ticks for a new enemy to appear
    static const int BONUS_MODE_TICKS; ///The time for bonus mode

    static const SDL_Rect GOLD_IMAGE; /// Rectangle holding the frame of the gold
    static const SDL_Rect CHERRY_IMAGE; /// Rectangle holding the frame of the cherry
    static const SDL_Rect EMERALD_IMAGE; /// Rectangle holding the frame of the emerald

    static const SDL_Point ENEMY_SPAWN_POSITION; /// Holds the spawn position of the enemies
    static const SDL_Point DIGGER_SPAWN_POSITION; /// Holds the spawn position of the player
    
    int m_newEnemyTicks; ///Ticks for new enemies to appear
    int m_enemiesCnt; /// Enemies that haven't appeared yet
    bool m_showCherry; /// Whether the cherry should be rendered
    bool m_isBonusMode; /// Whether the game is in bonus mode

    Timer m_nextEnemy; /// Keeps track of the time before next enemy should appear
    Timer m_bonusModeTimer; /// Keeps track of the time before the bonus mode should expire
    LTexture score; /// Texture for rendering the current score of the player
    Animation livesPlayer;

    Map* m_map; /// Holds a pointer to the map
    Digger m_player; 
    GameObject m_cherry;

    vector<Hobbin> m_enemies; /// Holds the current enemies
    vector<Gold> m_gold; /// Holds the current gold
    vector<GameObject> m_emeralds; /// Holds the current emeralds

    void checkCherryCollision();

    ///Calls objects' functions for checking for collidion
    void checkForCollision();
    /**
     * @brief Checks if it is time to create an enemy and if it is creates a new enemy.
     * After every creation the time for the next enemy increases
    */
    void createEnemy();
    //void removeEnemy(Character* enemy);

    /**
     * @brief Checks whether a sprite should be deleted and if so it is deleted.
    */
    void deleteSprites();
    /**
     * @brief Renders the score of the player
    */
    void renderFrame();
public:
    static const int MAX_LIVES; /// Max lives in the beggining for the player
    static const SDL_Color TEXT_COLOR; /// Holds the colour of the rendered text

    enum Objects {
        GOLD = 2,
        EMERALD,
        CHERRY
    };

    SpriteManager(); /// Default constructor

    /**
     * @brief Calls the render function of the sprites
    */
    void render();
    /**
     * @brief Sets the map of the sprites
    */
    void setMap(Map* map);
    /**
     * @brief Handles event and calls the corresponding function of the player
     * @param e - the event to be handled
    */
    void handleEvent(SDL_Event &e);

    /**
     * @brief Adds an object in the given position
     * @param index 
     * @param position 
    */
    void addObject(size_t index, const SDL_Point& position);

    /**
     * @brief Clears the states of the object(used for level reset)
    */
    void clear();
    /**
     * @brief used for transition between levels. Deletes old objects
    */
    void reset();
    /**
     * @return returns true if next level(and map + background) should be loaded
    */
    bool nextLevel();
    /**
     * @return returns true if the game is over and false otherwise
    */
    bool isGameOver();

};