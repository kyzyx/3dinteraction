#include "MouseKeyboardInterface.h"
#include "timestamp.h"
#include <SDL.h>
#include <Windows.h>

MouseKeyboardInterface::MouseKeyboardInterface() : RelativeInputInterface()
{
	status.inputType = InputStatus::MOUSEKBD;
	SDL_WM_GrabInput(SDL_GRAB_ON);
	ShowCursor(FALSE);
	SDL_ShowCursor(SDL_DISABLE);
	width = -1;
	height = -1;
}

MouseKeyboardInterface::~MouseKeyboardInterface()
{
	SDL_ShowCursor(SDL_ENABLE);
	ShowCursor(TRUE);
	SDL_WM_GrabInput(SDL_GRAB_OFF);
}

InputStatus MouseKeyboardInterface::updateDelta()
{
	InputStatus s = status;
	s.pos = Eigen::Vector3d(0,0,0);
	s.timestamp = timestamp();
	if (width < 0 && height < 0) {
		const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
		width = videoInfo->current_w;
		height = videoInfo->current_h;
		SDL_WarpMouse(width/2, height/2);
		return s;
	}
	if (SDL_GetAppState() & SDL_APPACTIVE) {
		int x,y;
		uint32_t buttons = SDL_GetRelativeMouseState(&x, &y);
		y = -y;
	
		uint8_t *keys = SDL_GetKeyState(NULL); // memory handled by SDL, do not free
		if (keys != NULL) {
			if (keys[SDLK_LSHIFT]) s.z() = -y/50.; // Move on XZ plane
			else {
				s.x() = x/50.;
				s.y() = y/50.;  // Move on XY plane
			}
		}
		if (buttons & SDL_BUTTON(1)) {
			s.setFlag(InputStatus::INPUTFLAG_SELECT);
		} else {
			s.flags = 0;
		}

		// Recenter the mouse in the screen
		SDL_GetMouseState(&x, &y);
		if (x < width/4 || x > width*3/4 || y < height/4 || y > height*3/4) {
			SDL_WarpMouse(width/2, height/2);
			SDL_Event events[5];
			SDL_PumpEvents();
			SDL_PeepEvents(events, 5, SDL_GETEVENT, SDL_EVENTMASK(SDL_MOUSEMOTION));
			SDL_GetRelativeMouseState(&x, &y);
		}
	}
	return s;
}