#pragma once
#include "MovingSprite.h"
class Hobbin;
class Digger;

class Bullet : public MovingSprite
{
private:
    /**
     * @brief Holds the rectangle in which is the first animation frame of the bullet
    */
    static const SDL_Rect ANIMATION_FRAME_1;
    /**
     * @brief Holds the rectangle in which is the second animation frame of the bullet
    */
    static const SDL_Rect ANIMATION_FRAME_2;

    /**
     * @brief Overrides the function from MoveSprite
     * @param direction - the tested direction
     * @return true if the bullet can move towards the direction and false otherwise
    */
    virtual bool tryToMove(const Direction& direction) override;
public:
    /**
     * @brief constructor taking the start position of the bullet
     * @param cell - the position of the bullet
    */
    Bullet(const Cell& cell);
    /**
     * @brief Sets move direction and start position
     * @param cell - sets the start position of the bullet
     * @param direction - sets the move direction
    */
    Bullet(const Cell& cell, const Direction& direction);

    /**
     * @brief Overrides the default render function
     * Renders only if the bullet is moving
     * That way the object can be reused instead of creating one each time someone shoots
    */
    virtual void render() override;

    /**
     * @brief Sets the move direction if the bullet is still
     * @param dir - the move direction
    */
    virtual void move(const Direction& dir) override;

    /**
     * @brief Resets the bullet so that it can be reused. 
     * Sets the start position and the direction in which it is shooted
     * @param cell - the start position
     * @param direction - the shoot direction
    */
    void reset(const Cell& cell, const Direction& direction);

    /**
     * @brief Handles enemy collision
     * Sets the enemy to be erased if the bullet collides with it.
     * Adds the points for killing an enemy to the player
    */
    void checkCollisionWithEnemy(vector<Hobbin>& enemies, Digger& player);
};

