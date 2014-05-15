#pragma once
#include "InputInterface.h"
#include "Leap.h"

class LeapInterface : public InputInterface
{
public:
	LeapInterface(void);
	~LeapInterface(void);

	virtual void update();

private:
	Leap::Controller controller;
	int tracked;
	Eigen::Quaterniond baserotation;
};

