#include "AdjustableInterface.h"


AdjustableInterface::~AdjustableInterface(void)
{
	if (input) delete input;
}

InputStatus AdjustableInterface::_getStatus (void) {
	status = input->getStatus();
	status.pos = t + status.pos;
	if (selected) {
		status.setFlag(InputStatus::INPUTFLAG_SELECT);
		selected = false;
	}
	return status;
}

void AdjustableInterface::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
	switch(sym) {
		case SDLK_ESCAPE:
			OnExit();
			break;
		case SDLK_w: 
			t.z() -= 1;
			break;
		case SDLK_s: 
			t.z() += 1;
			break;
		case SDLK_a: 
			t.x() -= 1;
			break;
		case SDLK_d: 
			t.x() += 1;
			break;
		case SDLK_e: 
			t.y() += 1;
			break;
		case SDLK_c: 
			t.y() -= 1;
			break;
		case SDLK_SPACE:
			selected = true;
			break;
	}
}

void AdjustableInterface::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
	if (sym == SDLK_SPACE) selected = false;
}