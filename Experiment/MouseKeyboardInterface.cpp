#include "MouseKeyboardInterface.h"
#include "timestamp.h"

MouseKeyboardInterface::MouseKeyboardInterface() : InputInterface()
{
	status.inputType = InputStatus::MOUSEKBD;
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
			status.z() += y - last_y;
		} else {
			// Move on XY plane
			status.x() += x - last_x;
			status.y() += y - last_y;
		}
	} else {
		status.x() = x - last_x;
		status.y() = y - last_y;
	}

	if (buttons & SDL_BUTTON(1)) {
		status.setFlag(InputStatus::INPUTFLAG_SELECT);
	} else {
		status.flags = 0;
	}

	status.timestamp = timestamp();

	last_x = x;
	last_y = y;
}

