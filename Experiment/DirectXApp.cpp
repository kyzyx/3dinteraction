#include "DirectXApp.h"
#include "D3DRenderer.h"

DirectXApp::DirectXApp(void) {
	running = true;
	screen = NULL;
	width = 1280;
	height = 720;
	flags = 0;
	fullscreen = false;
	handler = new SDLHandler(this);
	render = NULL;
}

DirectXApp::DirectXApp(bool fullScreen) {
	running = true;
	screen = NULL;
	width = 1280;
	height = 720;
	fullscreen = fullScreen;
	flags = 0;
	handler = new SDLHandler(this);
	render = NULL;
}

DirectXApp::~DirectXApp(void)
{
	if (handler) {
		delete handler;
		handler = NULL;
	}
	if (render) {
		delete render;
		render = NULL;
	}
}

bool DirectXApp::onInit(void) {
	if (!initSDL()) return false;
	try {
		render = new D3DRenderer(this);
	} catch (DirectXInitException e) {
		return false;
	}

	return true;
}

void DirectXApp::onRender() {
	render->clear();

	render->display();
}