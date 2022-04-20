#pragma once
#include "GameObject.h"
#include "Direction.h"
#include "Cell.h"
#include "Map.h"

class MovingSprite : public GameObject
{
protected:
	int m_velocity; ///holds the moving velocity
	bool moving; ///Whether the sprite is currently moving

	Map* m_map; ///Holds a pointer to the map

	//~Idea taken from the internet
	Direction m_moveDir; /// Holds the current move direction
	Direction m_nextDir; /// Holds the next move direction
	
	Cell currCell;
	Cell nextCell;

	virtual void updateDirection();
	virtual void updatePosition();
	//~
	///Checks whehter there is a wall in the cell in the given direction
	bool isWall(const Direction& direction);
	///Returns true if the sprite can move in the given direction and false otherwise
	virtual bool tryToMove(const Direction& direction);

public:
	/**
	 * @brief Creates a sprite with image clip holding the frame, the start points 
	 * @param imageClip - the animation frame
	 * @param points - the points of the sprite
	 * @param position - the start position of the sprite
	*/
	MovingSprite(const SDL_Rect& imageClip, int points, SDL_Point position = { 10, 10 });
	/**
	 * @brief Creates a sprite with points and a starting position
	*/
	MovingSprite(int points, SDL_Point position = { 10, 10 });

	/**
	 * @brief Sets m_nextDir to the given dir if possible to move this way
	 * @param dir - the direction in which we want the player to move next
	*/
	virtual void move(const Direction& dir);
	/**
	 * @brief Stops the sprite's movement
	*/
	void stopMoving();

	///Sets the map of the sprite
	virtual void setMap(Map* map);
	/**
	 * @brief Sets the velocity of the sprite
	*/
	void setVelocity(unsigned int velocity);

	/**
	 * @brief Renders the animation
	*/
	virtual void render() override;

	/**
	 * @brief Sets the sprite to be deleted
	*/
	virtual void erase();

	/**
	 * @brief Resets the movingInfo of the sprite
	 * @param direction 
	*/
	virtual void resetMovingInfo(const Direction& direction);

	/**
	 * @return true if the sprite is moving and false otherwise
	*/
	bool isMoving();
	/**
	 * @return The points of the sprite
	*/
	int getPoints();
	/**
	 * @return the current cell of the sprite 
	*/
	const Cell& getCurrentCell();
	/**
	 * @return the move direction of the sprite 
	*/
	const Direction& getMoveDirection() const;

};
