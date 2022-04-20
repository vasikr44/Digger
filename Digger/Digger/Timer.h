#pragma once

//#include <SDL.h>
#include "Include/SDL2-2.0.14/include/SDL.h"

//The code is taken from https://lazyfoo.net/tutorials/SDL/index.php

//The application time based timer
class Timer
{
private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;

public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();
};