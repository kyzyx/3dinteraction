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

	virtual void SDLupdate(SDL_Event* Event);
	virtual void update();

protected:
	Mesh* mesh;
};