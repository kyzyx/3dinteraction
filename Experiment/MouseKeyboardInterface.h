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
};