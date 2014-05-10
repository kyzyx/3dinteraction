#pragma once
#include "InputInterface.h"
#include "Mesh.h"

class MouseKeyboardInterface :
	public InputInterface
{
public:
	MouseKeyboardInterface();
	~MouseKeyboardInterface();

	virtual void update();

private:
	int last_x, last_y;
};