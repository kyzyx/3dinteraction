#pragma once
#include "SDLHandler.h"

struct InputStatus
{
	double x;
	double y;
	double z;
	double yaw;
	double pitch;
	double roll;

	bool buttonPressed;
};

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