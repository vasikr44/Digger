#include "Texture.h"
#include "Renderer.h"
#include "Cell.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <cmath>


SDL_Renderer* LTexture::gRenderer = NULL;

void LTexture::setRenderer(SDL_Renderer* renderer)
{
	LTexture::gRenderer = renderer;
}

LTexture::LTexture() : m_pitch(NULL), m_pixels(nullptr), m_texture(nullptr), m_width(0), m_height(0)
{}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface != NULL)
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_width = loadedSurface->w;
			m_height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	m_texture = newTexture;
	return m_texture != NULL;
}


bool LTexture::loadStreamableFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		Renderer& instance = Renderer::getInstance();
		//Convert surface to display format
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, instance.getWingowFormat(), 0);
		if (formattedSurface == NULL)
		{
			printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture(gRenderer, instance.getWingowFormat(), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
			if (newTexture == NULL)
			{
				printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Lock texture for manipulation
				SDL_LockTexture(newTexture, NULL, &m_pixels, &m_pitch);

				//Copy loaded/formatted surface pixels
				memcpy(m_pixels, formattedSurface->pixels, (formattedSurface->pitch) * (formattedSurface->h));

				//Unlock texture to update
				SDL_UnlockTexture(newTexture);
				m_pixels = NULL;

				//Get image dimensions
				m_width = formattedSurface->w;
				m_height = formattedSurface->h;
			}

			//Get rid of old formatted surface
			SDL_FreeSurface(formattedSurface);
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	SDL_LockTexture(newTexture, NULL, &m_pixels, &m_pitch);
	SDL_UnlockTexture(newTexture);

	//Return success
	m_texture = newTexture;
	return m_texture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(Renderer::font(), textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		throw std::runtime_error(TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		m_texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (m_texture == NULL)
		{
			throw std::runtime_error(SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_width = textSurface->w;
			m_height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return m_texture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		m_width = 0;
		m_height = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(m_texture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, int paddingY)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y + paddingY, m_width, m_height };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, m_texture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return m_width;
}

int LTexture::getHeight()
{
	return m_height;
}

bool LTexture::lockTexture()
{
	bool success = true;

	//Texture is already locked
	if (m_pixels != NULL)
	{
		return true;
	}
	//Lock texture
	else
	{
		if (SDL_LockTexture(m_texture, NULL, &m_pixels, &m_pitch) != 0)
		{
			printf("Unable to lock texture! %s\n", SDL_GetError());
			success = false;
		}
	}

	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if (m_pixels == NULL)
	{
		printf("Texture is not locked!\n");
		success = true;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture(m_texture);
		m_pixels = NULL;
		m_pitch = 0;
	}

	return success;
}

bool LTexture::dig(const SDL_Rect& position)
{
	if (!lockTexture())
	{
		printf("Unable to lock Foo' texture!\n");
		return false;
	}
	//Manual color key
	else
	{
		//Allocate format from window
		Uint32 format = Renderer::getInstance().getWingowFormat();
		SDL_PixelFormat* mappingFormat = SDL_AllocFormat(format);

		//Get pixel data
		Uint32* pixels = static_cast<Uint32*>(m_pixels);
		int pixelCount = (m_pitch / 4) * getHeight();

		//Map colors
		//Uint32 colorKey = SDL_MapRGB(mappingFormat, 0, 0xFF, 0xFF);
		Uint32 black = SDL_MapRGBA(mappingFormat, 0, 0, 0, 0);

		size_t r = Cell::CELL_SIZE/2;
		size_t cx = r + position.x;
		size_t cy = r + position.y;

		size_t r2 = r * r;
		size_t x = 0, x2 = 0, dx2 = 1;
		size_t y = r, y2 = y * y, dy2 = 2 * y - 1;
		size_t sum = r2;

		while (x <= y) {

			for (size_t i = (cx - x) + (cy - y) * Renderer::ScreenWidth(); i < (cx + x) + (cy - y) * Renderer::ScreenWidth(); i++)
			{
				if (i < pixelCount)
					pixels[i] = black;
			}
			for (size_t i = (cx - y) + (cy + x) * Renderer::ScreenWidth(); i < (cx + y) + (cy + x) * Renderer::ScreenWidth(); i++)
			{
				if (i < pixelCount)
					pixels[i] = black;
			}
			for (size_t i = (cx - y) + (cy - x) * Renderer::ScreenWidth(); i < (cx + y) + (cy - x) * Renderer::ScreenWidth(); i++)
			{
				if (i < pixelCount)
					pixels[i] = black;
			}

			for (size_t i = (cx - x) + (cy + y) * Renderer::ScreenWidth(); i < (cx + x) + (cy + y) * Renderer::ScreenWidth(); i++)
			{
				if (i < pixelCount)
					pixels[i] = black;
			}

			sum -= dx2;
			x2 += dx2;
			x++;
			dx2 += 2;
			if (sum <= y2) {
				y--; y2 -= dy2; dy2 -= 2;
			}
		}

		//Unlock texture
		unlockTexture();

		//Free format
		SDL_FreeFormat(mappingFormat);
	}

	return true;
}
