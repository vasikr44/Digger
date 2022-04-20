#include "Animation.h"
#include <iostream>

const int Animation::ADDITIONAL_FRAMES_PER_IMAGE = 20;
LTexture Animation::spritesTexture;

void Animation::loadTexture(const char* path)
{
	spritesTexture.loadFromFile(path);
}


Animation::Animation(const Animation& other) 
	: m_currentFrame(other.m_currentFrame), m_angle(other.m_angle), m_center(other.m_center), m_flip(other.m_flip), 
	  m_frameRate(other.m_frameRate), m_frames(other.m_frames), m_frame(other.m_frame), time(other.time)
{}

Animation::Animation(const SDL_Rect& image) 
	: m_currentFrame(0), m_angle(0.0), m_center(nullptr), m_flip(SDL_FLIP_NONE), 
	  m_frameRate(15), m_frame({ 0, 0, Cell::CELL_SIZE, Cell::CELL_SIZE })
{
	m_frame = image;
	m_frames.push_back(image);

	time.start();
}


Animation::Animation()
	: m_currentFrame(0), m_angle(0.0), m_center(nullptr), m_flip(SDL_FLIP_NONE), 
	  m_frameRate(0), m_frame({ 0, 0, Cell::CELL_SIZE, Cell::CELL_SIZE })
{
	time.start();
}

Animation::~Animation()
{
	delete m_center;
}

int Animation::getWidth()
{
	return m_frames[m_currentFrame].w;
}

int Animation::getHeight()
{
	return m_frames[m_currentFrame].h;
}

void Animation::addFrame(const SDL_Rect& frame)
{
	if (m_frames.empty())
	{
		m_frame = frame;
	}

	m_frames.push_back(frame);
	m_frameRate += ADDITIONAL_FRAMES_PER_IMAGE;
}

void Animation::nextFrame()
{
	++m_currentFrame;

	if (m_currentFrame >= m_frames.size()) 
	{
		m_currentFrame = 0;
	}
	m_frame = m_frames[m_currentFrame];
}

void Animation::render(int x, int y, int paddingY)
{
	if (m_frameRate <= time.getTicks())
	{
		nextFrame();
		time.start();
	}
	spritesTexture.render(x, y, &m_frame, m_angle, m_center, m_flip, paddingY);
}


void Animation::flip(const SDL_RendererFlip& flip)
{
	m_flip = flip;
}

void Animation::rotate(double angle, SDL_Point* center)
{
	m_angle = angle;
	m_center = center;
}
