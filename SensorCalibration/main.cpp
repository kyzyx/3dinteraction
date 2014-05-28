#include "CalibrationApp.h"
#include "timestamp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	double startTime = timestamp();
	App* app = new CalibrationApp();
	return app->onExecute();
}