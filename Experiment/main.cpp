#include "DirectXApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	App* app = new DirectXApp();
	return app->onExecute();
}