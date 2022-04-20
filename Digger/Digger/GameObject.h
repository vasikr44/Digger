#pragma once
//#include <SDL.h>
#include "Include/SDL2-2.0.14/include/SDL.h"
#include "Animation.h"

class SpriteManager;

class GameObject
{
protected:
    SDL_Point m_pos; /// Current position of the object
    int m_points; /// The points of the object
    bool m_delete; /// Whether the object should be deleted

    /// Holds the animation of the object
    Animation m_animation; 

    friend class SpriteManager;

public:
    /**
     * @brief Constructor creating an object with the same properties as other
    */
    GameObject(const GameObject& other);
    /**
     * @brief Creates a game object
     * @param imageClip - the position of the frame of the animation
     * @param points - the points of the object
     * @param position - the start position of the object
    */
    GameObject(const SDL_Rect& imageClip, int points, SDL_Point position = { 100, 100 });
    /**
     * @brief Creates an object
     * @param points - the points of the object
     * @param position - the position of the object
    */
    GameObject(int points, SDL_Point position = { 100, 100 });

    /**
     * @brief renders the object
    */
    virtual void render();

    /**
     * @brief sets m_delete to true;
    */
    virtual void deleteObject();

    /**
     * @brief adds a frame to the animation
     * @param imageClip - the position of the frame in the image
    */
    void addFrame(const SDL_Rect& imageClip);

    /**
     * @return the x-axis position of the object
    */
    int getX() const;

    /**
     * @return the y-axis position of the object
    */
    int getY() const;

    /**
     * @brief Sets the position x of the object to the given value
    */
    void setX(int x);
    /**
     * @brief Sets the position y of the object to the given value
    */
    void setY(int y);

    /**
     * @brief Returns the value of m_delete
     * @return true if the object should be deleted and false otherwise
    */
    virtual bool shouldBeDeleted();
    /**
     * @return returns the points of the objects
    */
    int getPoints() const;
    /**
     * @return the position of the top left corner of the object
    */
    const SDL_Point& getPosition() const;
};

