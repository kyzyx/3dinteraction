#include "MouseKeyboardInterface.h"
#include "timestamp.h"
#include <cstdio>

MouseKeyboardInterface::MouseKeyboardInterface() : InputInterface()
{
	status.inputType = MOUSEKBD;
}

MouseKeyboardInterface::~MouseKeyboardInterface()
{
}

void MouseKeyboardInterface::update()
{
    int x,y;	
	uint32_t buttons = SDL_GetMouseState(&x, &y);
	uint8_t *keys = SDL_GetKeyState(NULL); // memory handled by SDL, do not free
	if (keys != NULL) {
		if (keys[SDLK_LSHIFT]) {
			// Move on Z plane
			status.z = y;
		} else {
			// Move on XY plane
			status.x = x;
			status.y = y;
		}
	} else {
		status.x = x;
		status.y = y;
	}
	status.buttonPressed = buttons&SDL_BUTTON(1);
	status.timestamp = timestamp();
}

