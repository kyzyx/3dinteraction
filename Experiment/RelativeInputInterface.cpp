#include "RelativeInputInterface.h"
#include "InteractionSpace.h"
#include "windows.h"

void RelativeInputInterface::update(void) {
	InputStatus s = updateDelta();
	s.pos += status.pos;
	s.pos = InteractionSpace::closestPointInVolume(s.pos.cast<float>()).cast<double>();

	s.rot = s.rot * status.rot;
	status = s;
}