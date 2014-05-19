#pragma once
#include "InputStatus.h"

class InputInterface
{
public:
	InputInterface() : status() {}
	virtual ~InputInterface() {}

	virtual void update() {}

	InputStatus getStatus(void) {return _getStatus();}

protected:
	virtual InputStatus _getStatus (void) { return status; }
	InputStatus status;
};