#pragma once
#include "SDLHandler.h"
#include "InputInterface.h"

class InputHandler :
	public SDLHandler
{
public:
	InputHandler();
	InputHandler(InputInterface* Input);
	~InputHandler();

	virtual void OnEvent(SDL_Event* Event);

protected:
	InputInterface* input;
};