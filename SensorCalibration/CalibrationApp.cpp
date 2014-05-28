#include "CalibrationApp.h"

CalibrationApp::CalibrationApp(void) : ExperimentApp()
{
	experiment = new CalibrationExperiment();
}


CalibrationApp::~CalibrationApp(void)
{
}


void CalibrationApp::onLoop(void) {
    experiment->onLoop();
	if (scene->finished()) {
		OutputDebugString("Next scene!\n");
		scene = experiment->getNextScene();
		if (scene) scene->init((D3DRenderer*)render);
		else {
			running = false;
			CalibrationExperiment* exp = (CalibrationExperiment*) experiment;
			exp->writeTransform();
		}
	} else {
		InputStatus input = experiment->getInput();
				std::wstringstream msg;
		msg	<< L"pos=("
			<< input.x() << L", "
			<< input.y() << L", "
			<< input.z()
			<< L")";
		render->drawText(msg.str().c_str(), 300, 160, 0xffffffff, 20);
		scene->processInput(input);
	}
}
