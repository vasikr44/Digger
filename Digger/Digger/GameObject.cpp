#include "GameObject.h"
#include <iostream> 
#include <SDL.h>

const LTexture texture;

GameObject::GameObject(const GameObject& other)
    : m_animation(other.m_animation), m_points(other.m_points), m_pos(other.m_pos), m_delete(false)
{

}

GameObject::GameObject(const SDL_Rect& imageClip, int points, SDL_Point position)
    : m_animation(imageClip), m_points(points), m_pos(position), m_delete(false)
{}

GameObject::GameObject(int points, SDL_Point position)
    : m_points(points), m_pos(position), m_delete(false)
{}


void GameObject::deleteObject()
{
    m_delete = true;
}

void GameObject::addFrame(const SDL_Rect& imageClip)
{
    m_animation.addFrame(imageClip);
}

int GameObject::getX() const
{
    return m_pos.x;
}

int GameObject::getY() const
{
    return m_pos.y;
}

void GameObject::setX(int x)
{
    m_pos.x = x;
}

void GameObject::setY(int y)
{
    m_pos.y = y;
}

void GameObject::render()
{
    m_animation.render(m_pos.x, m_pos.y);
}


const SDL_Point& GameObject::getPosition() const
{
    return m_pos;
}

int GameObject::getPoints() const
{
    return m_points;
}


bool GameObject::shouldBeDeleted()
{
    return m_delete;
}