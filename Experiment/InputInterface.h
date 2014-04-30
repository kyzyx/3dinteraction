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

	virtual void updateStatus(SDL_Event* Event){}

	InputStatus* getStatus() {return status;}

protected:
	InputStatus* status;
};