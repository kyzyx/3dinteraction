#include "InputHandler.h"

InputHandler::InputHandler()
{
}

InputHandler::InputHandler(InputInterface* Input)
{
	input = Input;
}

void InputHandler::OnEvent(SDL_Event* Event) {
	input->updateStatus(Event);
	SDLHandler::OnEvent(Event);
}