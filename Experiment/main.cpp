#include "ExperimentApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	App* app = new ExperimentApp();
	return app->onExecute();
}