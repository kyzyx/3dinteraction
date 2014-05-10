#include "HydraInterface.h"
#include <Windows.h>
#include <sixense.h>
#include <sixense_utils/button_states.hpp>
#include <sixense_utils/controller_manager/controller_manager.hpp>
#include "timestamp.h"

HydraInterface::HydraInterface()
{
	int STATE;

	// Init sixense
	STATE = sixenseInit();
	if(STATE == SIXENSE_FAILURE)
	{
		OutputDebugString("Init Failed.");
	}

	// Init the controller manager. This makes sure the controllers are present, assigned to left and right hands, and that
	// the hemisphere calibration is complete.
	sixenseUtils::getTheControllerManager()->setGameType( sixenseUtils::ControllerManager::ONE_PLAYER_TWO_CONTROLLER );

	status.inputType = HYDRA;
}

HydraInterface::~HydraInterface()
{
	sixenseExit();
}

void HydraInterface::update()
{
	int STATE;

	// update the controller manager with the latest controller data here
	STATE = sixenseSetActiveBase(0);
	if(STATE == SIXENSE_FAILURE)
	{
		OutputDebugString("Set Active Base Failed.");
	}

	sixenseAllControllerData acd;
	STATE = sixenseGetAllNewestData( &acd );
	if(STATE == SIXENSE_FAILURE)
	{
		OutputDebugString("Get All Newest Data Failed.");
	}

	sixenseUtils::getTheControllerManager()->update( &acd );

	// Ask the controller manager which controller is on the left
	// (Donny: this doesn't actually seem to distinguish between left and right.
	//         Even if you look for "P1R" -- it always returns the left one)
	int index = sixenseUtils::getTheControllerManager()->getIndex( sixenseUtils::ControllerManager::P1L );

	status.pos[0] = acd.controllers[index].pos[0];
	status.pos[1] = -acd.controllers[index].pos[1];
	status.pos[2] = acd.controllers[index].pos[2];

	status.rot = Eigen::Quaterniond(acd.controllers[index].rot_quat[3], acd.controllers[index].rot_quat[0], -acd.controllers[index].rot_quat[1], -acd.controllers[index].rot_quat[2]);

	static sixenseUtils::ButtonStates states;
	states.update( &acd.controllers[index] );

	// Do something if the trigger was pulled
	if( states.triggerJustPressed() ) {
		status.flags = InputStatus::INPUTFLAG_SELECT;
	}

	// Do something if the trigger was released
	if( states.triggerJustReleased() ) {
		status.flags = InputStatus::INPUTFLAG_DESELECT;
	}

	status.timestamp = timestamp();
}