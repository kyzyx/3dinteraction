#pragma once
#include "SDLHandler.h"
#include "InputStatus.h"

class InputInterface
{
public:
	InputInterface(){}
	~InputInterface(){}

	virtual void SDLupdate(SDL_Event* Event){}
	virtual void update(){}

	InputStatus* getStatus() {return status;}

protected:
	InputStatus* status;
};