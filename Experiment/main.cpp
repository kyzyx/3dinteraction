#include "ExperimentApp.h"
#include "timestamp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	double startTime = timestamp();
	App* app = new ExperimentApp();
	int ret = app->onExecute();
	delete app;
	return ret;
}