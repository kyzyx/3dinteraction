#pragma once
#include "InputInterface.h"
#include "Mesh.h"

class MouseKeyboardInterface :
	public InputInterface
{
public:
	MouseKeyboardInterface();
	~MouseKeyboardInterface();

	virtual void SDLupdate(SDL_Event* Event);
	virtual void update();

protected:
	Mesh* mesh;
};