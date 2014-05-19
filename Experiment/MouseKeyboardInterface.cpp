#include "MouseKeyboardInterface.h"
#include "timestamp.h"
#include <SDL.h>
#include <Windows.h>

MouseKeyboardInterface::MouseKeyboardInterface() : InputInterface()
{
	status.inputType = InputStatus::MOUSEKBD;
	SDL_GetMouseState(&last_x, &last_y);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	ShowCursor(FALSE);
	SDL_ShowCursor(SDL_DISABLE);
}

MouseKeyboardInterface::~MouseKeyboardInterface()
{
	SDL_ShowCursor(SDL_ENABLE);
	ShowCursor(TRUE);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
}

void MouseKeyboardInterface::update()
{
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
	int width = videoInfo->current_w;
	int height = videoInfo->current_h;
    int x,y;	
	uint32_t buttons = SDL_GetRelativeMouseState(&x, &y);
	y = -y;
	
	uint8_t *keys = SDL_GetKeyState(NULL); // memory handled by SDL, do not free
	if (keys != NULL) {
		if (keys[SDLK_LSHIFT]) {
			// Move on Z plane
			double dz = y;// - last_y;
			status.z() -= dz / height / 2.5;
		} else {
			// Move on XY plane
			double dx = x;// - last_x;
			double dy = y;// - last_y;
			status.x() += dx / height * (width/height) / 2.5;
			status.y() += dy / height / 2.5;
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

	// Recenter the mouse in the screen
	SDL_GetMouseState(&x, &y);
	if (x < 10 || x > width-10 || y < 10 || y > height-10) {
		SDL_WarpMouse(width/2, height/2);
		SDL_Event events[5];
		SDL_PumpEvents();
		SDL_PeepEvents(events, 5, SDL_GETEVENT, SDL_EVENTMASK(SDL_MOUSEMOTION));
		SDL_GetRelativeMouseState(&x, &y);
	}

}

