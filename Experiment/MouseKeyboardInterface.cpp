#include "MouseKeyboardInterface.h"
#include "timestamp.h"

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
			status.pos[2] += y - last_y;
		} else {
			// Move on XY plane
			status.pos[0] = x;
			status.pos[1] = y;
		}
	} else {
		status.x() = x;
		status.y() = y;
	}
	status.flags = buttons&SDL_BUTTON(1) ? InputStatus::INPUTFLAG_SELECT : InputStatus::INPUTFLAG_NONE;
	status.timestamp = timestamp();

	last_x = x;
	last_y = y;
}

