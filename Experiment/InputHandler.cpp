#include "InputHandler.h"

InputHandler::InputHandler()
{
}

InputHandler::InputHandler(InputInterface* Input)
{
	input = Input;
}

void InputHandler::OnEvent(SDL_Event* Event) {
	input->SDLupdate(Event);
	SDLHandler::OnEvent(Event);
}

void InputHandler::update()
{
	input->update();
}