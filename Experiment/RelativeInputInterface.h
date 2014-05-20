#pragma once
#include "inputinterface.h"
class RelativeInputInterface :
	public InputInterface
{
public:
	RelativeInputInterface(void) {}
	~RelativeInputInterface(void) {}

	virtual void update(void);
	virtual void reset(void) { status = InputStatus(); }

protected:
	virtual InputStatus updateDelta (void) = 0;
};

