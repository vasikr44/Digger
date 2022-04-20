#pragma once
#include "MovingSprite.h"
#include "PathFinder.h"
#include "Timer.h"

class Hobbin : public MovingSprite
{
private:
	static const int MAX_TIME_AS_NOBBIN;  /// The maximum time as nobbin
	static int requiredCrossesForNobbin; /// The number of crosses required for an enemy to become a nobbin
	static int timesEnemiesCrossed; /// Keeps track of the number of crosses

	Pair destination; /// Holds the destination of the hobbin
	Pair lastDestination; /// Holds the last destination of the nobbin
	PathFinder<Map::MAP_WIDTH, Map::MAP_HEIGHT> pathfinder; /// Object of type PathFinder that calculates the path

	Timer timeAsNobbin; /// Timer that keeps track of the time as nobbin
	Animation nobbinAnimation; /// Holds the nobbin animation

	bool nobbin; /// Whether the sprite is currently nobbin
	void setMoveDirection(); /// Sets move direction of the sprite
	void setDirectionFromPathfinder(); /// Sets the direction taking it from the pathfinder result

	void findPath(); /// The pathfinder finds a path to the destination

public:
	/**
	 * @brief Constructor taking the image of the frame and the start position
	*/
	Hobbin(SDL_Rect& imageClip, SDL_Point position);
	/**
	 * @brief Constructor taking the start position
	*/
	Hobbin(SDL_Point position);

	/**
	 * @brief renders the current animation of the sprite
	*/
	virtual void render() override;
	/**
	 * @brief Sets the destination of the sprite
	*/
	void setDestination(size_t x, size_t y);

	/**
	 * @brief Checks for collision with objects. Uses checkCollision
	 * If it collides they disappear
	*/
	void checkCollisionWithObjects(vector<GameObject>& objects);

	/**
	 * @brief Checks whether the hobbin is colliding with an object.
	 * If it is the object is set to be deleten
	*/
	bool checkCollision(GameObject& object);
	/**
	 * @brief Counts the number of collisions with other hobbins
	*/
	void checkCollisionWithHobbin(vector<Hobbin>& hobbins);

	/**
	 * @brief returns true if the nobbin has reached his destination
	*/
	bool reachedGoal();
	/**
	 * @brief returbs true if the sprite is a nobbin
	*/
	bool isNobbin();
};