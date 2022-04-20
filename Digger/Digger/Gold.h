#pragma once
#include "MovingSprite.h"
#include "Timer.h"
#include "Hobbin.h"
#include "Digger.h"

class Gold : public MovingSprite
{
private:
    static const SDL_Rect COIN_IMAGE; /// Rectangle with the position of the coin image in the sprite image
    static int coinsTicks; /// ticks as coins before they disappear
    static int delayBeforeFalling; ///holds the ticks that the gold has to wait before it fallse

    int falling_blocks; /// The count of the blocks it has fallen
    bool m_isCoins; /// Whether the object has turned to coins
    bool m_isFalling; /// Whether the object is falling

    Timer timeAsCoins; /// Keeps track of the time as coins
    Timer delayForFalling; /// Keeps track of the time before the gold falls

    Animation m_coinsAnimation; /// Holds the coins animation

    /**
     * @brief Checks if the gold can move in a given direction.
     * If the gold is falling it can only move down. Otherwise it can move to the left and to the right as well
    */
    virtual bool tryToMove(const Direction& direction) override;

    /**
     * @brief Returns true if the gold is colliding with a sprite while falling.
     * If it collides with the player he dies
     * If it collides with an enemy the player takes the money of the enemy
    */
    bool handleFallingCollision(MovingSprite& sprite, Digger& player);
    /**
     * @brief Checks for collision while the gold is in coins state
     * If it collides with the player the player takes the points of the gold
     * If it collides with an enemy the coins just disappear
    */
    bool handleCoinsCollision(MovingSprite& sprite, Digger& player);
    /**
     * @brief Checks if the gold is in basic state and if a sprite is trying to push it, it moves.
     * Stops a sprite from moving on top of it
    */
    bool handleBasicStateCollision(MovingSprite& sprite);
    /**
     * @brief Checks whether a sprite is under the bag
    */
    bool checkUnder(MovingSprite& sprite);
    /**
     * @brief Checks if the sprite is a horizontal neighbout of the gold
    */
    bool checkHorizontal(MovingSprite& sprite);
    /**
     * @brief Helper function for checkUnder
    */
    bool isUnder(MovingSprite& sprite);

    /**
     * @brief Updates the state of the bag
     * If it's set to falling but it has reached the ground
     * It checks whether the falling blocks are more than one
     * And turns the gold into coins or returns to basic state
    */
    void updateState();
    /**
     * @brief Checks if the cell under the gold is empty
     * Only checking when the gold is moving to the left or to the right
     * So it can fall imediately if it is over an empty cell
    */
    void checkForEnptyCell();
    /**
     * @brief returns back to basic state
    */
    void backToBasicState();

public:
    /**
     * @brief Constructor setting the start position of the gold
    */
    Gold(SDL_Point position);
    /**
     * @return true if the gold is coins and false otherwise
    */
    bool isCoins();

    /**
     * @brief sets the gold to falling state
    */
    void fall();
    /**
     * @brief renders the current animation of the onject
    */
    virtual void render() override;
    /**
     * @brief Updates the position of the gold
    */
    virtual void updatePosition() override;
    /**
     * @brief Updates the direction of the gold
    */
    virtual void updateDirection() override;

    /**
     * @brief Checks for collision with the sprites. 
     * Calls the private collision functions for the different object states
    */
    void checkCollisionWithSprites(vector<Hobbin>& enemies, Digger& player);

    /**
     * @brief Resets the state of the bag to basic state.
    */
    void reset();
};

