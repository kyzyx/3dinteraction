#pragma once
#include "RelativeInputInterface.h"

class MouseKeyboardInterface :
	public RelativeInputInterface
{
public:
	MouseKeyboardInterface();
	~MouseKeyboardInterface();

protected:
	virtual InputStatus updateDelta();
	int width, height;
};