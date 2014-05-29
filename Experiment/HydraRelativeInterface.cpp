#include "HydraRelativeInterface.h"
#include <sixense.h>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>
#include "timestamp.h"
#include <Windows.h>

HydraRelativeInterface::HydraRelativeInterface(void) : RelativeInputInterface()
{
	if (sixenseInit() == SIXENSE_FAILURE) {
		OutputDebugString("Init Failed.");
	}
	sixenseUtils::getTheControllerManager()->setGameType( sixenseUtils::ControllerManager::ONE_PLAYER_TWO_CONTROLLER );
	status.inputType = InputStatus::HYDRA;
}


HydraRelativeInterface::~HydraRelativeInterface(void)
{
	sixenseExit();
}

InputStatus HydraRelativeInterface::updateDelta(void)
{
	InputStatus s;
	s.flags = 0;
	s.timestamp = timestamp();
	s.inputType = InputStatus::HYDRA;

	int STATE;
	STATE = sixenseSetActiveBase(0);
	if (STATE == SIXENSE_FAILURE) {
		OutputDebugString("Set Active Base Failed.");
	}
	sixenseAllControllerData acd;
	STATE = sixenseGetAllNewestData( &acd );
	if (STATE == SIXENSE_FAILURE) {
		OutputDebugString("Get All Newest Data Failed.");
	}
	sixenseUtils::getTheControllerManager()->update( &acd );
	int index = sixenseUtils::getTheControllerManager()->getIndex( sixenseUtils::ControllerManager::P1L );
	static sixenseUtils::ButtonStates states;
	states.update( &acd.controllers[index] );
	
	if (states.triggerJustPressed()) {
		s.flags = InputStatus::INPUTFLAG_SELECT;
	}
	if (states.triggerJustReleased()) {
		s.flags = InputStatus::INPUTFLAG_DESELECT;
	}
	
	if (acd.controllers[index].buttons & SIXENSE_BUTTON_1) {
		if (!states.buttonJustPressed(SIXENSE_BUTTON_1)) {
			const double MOUSE_THRESHOLD = 0.08;
			s.pos[0] = acd.controllers[index].pos[0]/10; // mm to cm
			s.pos[1] = acd.controllers[index].pos[1]/10;
			s.pos[2] = acd.controllers[index].pos[2]/10;

			s.pos -= originpos;

			if (s.pos.norm() > MOUSE_THRESHOLD) {
				s.pos *= 1.6;
				s.setFlag(InputStatus::INPUTFLAG_DEBUG);
			}

			s.rot = Eigen::Quaterniond(acd.controllers[index].rot_quat[3], acd.controllers[index].rot_quat[0], acd.controllers[index].rot_quat[1], acd.controllers[index].rot_quat[2]);
			s.rot = s.rot*originrot.inverse();
		}
		originpos[0] = acd.controllers[index].pos[0]/10; // mm to cm
		originpos[1] = acd.controllers[index].pos[1]/10;
		originpos[2] = acd.controllers[index].pos[2]/10;

		originrot = Eigen::Quaterniond(acd.controllers[index].rot_quat[3], acd.controllers[index].rot_quat[0], acd.controllers[index].rot_quat[1], acd.controllers[index].rot_quat[2]);
	}

	return s;
}
