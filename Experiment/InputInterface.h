#pragma once
#include "SDLHandler.h"

typedef enum {MOUSEKBD, HYDRA, LEAP} InputType;

struct InputStatus
{
	double x;
	double y;
	double z;
	double yaw;
	double pitch;
	double roll;

	bool buttonPressed;

	InputType inputType; // tag where this data came from
	double timestamp;    // when was this event generated?

	InputStatus operator- (const InputStatus &other) const {
		InputStatus is;
		is.x = x - other.x;
		is.x = y - other.y;
		is.x = z - other.z;
		is.yaw = yaw - other.yaw;
		is.pitch = pitch - other.pitch;
		is.roll = roll - other.roll;
		is.buttonPressed = buttonPressed != other.buttonPressed;
		is.timestamp = timestamp - other.timestamp;
		return is;
	}
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