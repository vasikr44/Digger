#pragma once
#include "MovingSprite.h"
#include "Hobbin.h"
#include "Timer.h"
#include "Bullet.h"

class Digger : public MovingSprite
{
private:
	static const int POINTS_FOR_EATEN_ENEMY; /// Points the player gets for eating an enemy

	Bullet m_bullet; /// Holds a bullet that can be shooted
	int m_livesLeft; /// Holds the remaining lifes
	
	Animation m_chargingAnimation; /// Animation for the charging player
	Animation* m_currentAnimation; /// Holds a pointer to the current animation

	size_t m_chargingTime; ///Holds the charging time for the player to be able to shoot again
	Timer m_charging; /// Timer that keeps track of the charging time

	size_t m_consecutiveEmeralds; /// Should store th number of consecutive eaten emeralds
	size_t m_bonusModeKillCount; /// If in bonus mode calculates the points taken for eating an enemy

	void setChargingFrames();  /// Shoots a bullet and sets the animation to charging

	/**
	 * @brief Implements its own version of tryToMove overriden from MovingSprite
	 * @param direction - the direction we are trying to move towards
	 * @return true if the player can move in this direction and false otherwise
	*/
	virtual bool tryToMove(const Direction& direction) override;

public:
	/**
	 * @brief Constructor taking an image clip for an animation frame and a start position
	 * @param imageClip - rectangle for the animation frame
	 * @param position - start position
	*/
	Digger(SDL_Rect& imageClip, SDL_Point position);
	/**
	 * @brief Only sets the start position of the dplayer
	*/
	Digger(SDL_Point position);

	/**
	 * @brief renders the current animation and the bullet
	*/
	virtual void render() override;
	/**
	 * @brief reduces the lives of the player.
	*/
	virtual void erase() override;
	/**
	 * @brief Sets the map variable
	 * @param map - the map of the level
	*/
	virtual void setMap(Map* map) override;
	/**
	 * @brief Updates the position of the player
	*/
	virtual void updatePosition() override;

	/**
	 * @brief Resets the player's info. Mostly used when continuing to next level
	 * The player doesn't lose its points
	 * @param position - sets the start postion
	*/
	void reset(const SDL_Point& position);
	/**
	 * @brief Adds points to the player
	*/
	void addPoints(size_t points);
	/**
	 * @brief If the player is not charging it shoots a bullet in the current direction 
	 * and sets the state to charging
	 * @param isCharging - sets the charging
	*/
	void setCharging(bool isCharging);

	/**
	 * @return the number of lives left
	*/
	int getLivesLeft();

	/**
	 * @return true if the player is alive, false otherwise
	*/
	bool isAlive();
	/**
	 * @return true if the player is moving and false otherwise
	*/
	bool isMoving();
	/**
	 * @return returns true if the player is charging
	*/
	bool isCharging();
	/**
	 * @brief Checks whether the player shou;d be deleted
	 * @return true if the player is dead and doesn't have more lives and false otherwise
	*/
	virtual bool shouldBeDeleted() override;

	/**
	 * @brief Checks collision with the enemies. If in bonus mode the player eats the enemies. 
	 * Otherwise if they are colliding the player loses a life and the level restarts(all progres is saved)
	*/
	void checkCollisionWithHobbin(vector<Hobbin>& enemies, bool isBonusMode = false);
	/**
	 * @brief Checks collision with objects. Uses checkCollision
	 * @see checkCollision(GameObject& object)
	 * @param objects 
	*/
	void checkCollisionWithObjects(vector<GameObject>& objects);

	/**
	 * @brief Checks collision with the object.
	 * If colliding eats the object. Sets it to be erased and takes its points
	 * @return true if the object was eaten and false otherwise
	*/
	bool checkCollision(GameObject& object);
};

