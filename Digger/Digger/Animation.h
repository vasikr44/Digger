#pragma once
//#include <SDL.h>
#include "Include/SDL2-2.0.14/include/SDL.h"
#include <vector>
#include "Cell.h"
#include "Texture.h"
#include "Timer.h"

using std::vector;

class Animation
{
private:
	static const int ADDITIONAL_FRAMES_PER_IMAGE;
	/// holds LTexture object that manages the raw texture
	static LTexture spritesTexture; 
	Timer time; /// Timer used for calculating the frame rate

	int m_currentFrame; /// The index of the current frame that needs to be rendered
	unsigned int m_frameRate;  /// Variable storing the frame rate per milisecond
	
	SDL_Rect m_frame; /// Rectangle holding a position in the image from which the animation takes the frame
	vector<SDL_Rect> m_frames; ///Vector holding all the given frames

	double m_angle; /// The angle at which the frame should be rendered
	SDL_Point* m_center; /// The center point of rotation
	SDL_RendererFlip m_flip; /// The flip of the frame

	void nextFrame(); /// Function loading the nextFrame
	

public:
	/**
	 * @brief Loads spritesTexture
	 * @param path - takes the path to the sprites'
	*/
	static void loadTexture(const char* path);
	
	/**
	 * @brief Copy constructor
	 * @param other - creates an animation that holds the same frames as other
	*/
	Animation(const Animation& other);
	/**
	 * @brief - sets one frame
	 * @param image - the rectangle of the given animation
	*/
	Animation(const SDL_Rect& image);
	/**
	 * @brief default constructor
	*/
	Animation();
	/**
	 * @brief default constructor
	*/
	~Animation();

	int getWidth(); /// returns the width of the animation
	int getHeight(); /// returns the height the animation

	/**
	 * @brief Adds now frame in the animation
	 * @param frame - the rectangle is the position of the frame in the image
	*/
	void addFrame(const SDL_Rect& frame);
	/**
	 * @brief Calls Renderer and renders the current frame in the given position
	 * Renders with the default padding
	 * @param x - the x-axis value of the left corner of the image 
	 * @param y - the y-axis value of the left corner of the image 
	*/
	void render(int x, int y, int paddingY = Cell::CELL_SIZE);


	void flip(const SDL_RendererFlip& flip);  ///sets the flip of the animation
	void rotate(double angle, SDL_Point* center = nullptr); ///sets the rotation
};

