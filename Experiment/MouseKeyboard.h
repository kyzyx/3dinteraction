#pragma once
#include "InputInterface.h"
#include "Mesh.h"

class MouseKeyboard :
	public InputInterface
{
public:
	MouseKeyboard();
	MouseKeyboard(Mesh* mesh);
	~MouseKeyboard();

	virtual void updateStatus(SDL_Event* Event);

protected:
	Mesh* mesh;
};