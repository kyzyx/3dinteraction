#include <string>

#include "ExperimentApp.h"
#include "timestamp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	double startTime = timestamp();
	std::string experimentConfig(pScmdline);
	if (experimentConfig.size() == 0) {
		experimentConfig = "exp1.json";
	}
	App* app = new ExperimentApp(experimentConfig);
	int ret = app->onExecute();
	delete app;
	return ret;
}