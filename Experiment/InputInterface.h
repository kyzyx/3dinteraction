#pragma once
#include "SDLHandler.h"

typedef enum {MOUSEKBD, HYDRA, LEAP, UNKNOWN} InputType;

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

	InputStatus (void)
		: x(0), y(0), z(0), yaw(0), pitch(0), roll(0),
		inputType(UNKNOWN), timestamp(0.0), buttonPressed(false) {}

	InputStatus operator- (const InputStatus &other) const {
		InputStatus is;
		is.inputType = inputType; // save type of left operatand
		is.x = x - other.x;
		is.y = y - other.y;
		is.z = z - other.z;
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
	InputInterface() {}
	~InputInterface() {}

	virtual void update(){}

	InputStatus getStatus() {return status;}

protected:
	InputStatus status;
};