#include <SDL.h>
#include "app.h"
#include "timestamp.h"

App::App() {
	running = true;
	screen = NULL;
	width = 640;
	height = 480;
	fullscreen = false;
	flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
	handler = new SDLHandler(this);
}

App::App(int w, int h, SDLHandler* myhandler) {
	running = true;
	screen = NULL;
	width = w;
	height = h;
	flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
	handler = myhandler;
}

App::~App (void) {
}

void App::stop() {
	running = false;
}

int App::onExecute() {
	if (!onInit()) stop();

	SDL_Event ev;
	loopTime = 0.0;
	double lastNow = timestamp();
	while (running) {
		// Calculate time delta since this loop last ran
        double now = timestamp();		
		loopTime = now - lastNow;
		lastNow = now;

		while (SDL_PollEvent(&ev)) {
			if (handler) {
				handler->OnEvent(&ev);
				for (size_t i = 0; i < handlers.size(); ++i) handlers[i]->OnEvent(&ev);
			}
		}
		if (handler) {
			handler->update();
			for (size_t i = 0; i < handlers.size(); ++i) handlers[i]->update();
		}
		onLoop();
		onRender();
	}
	onCleanup();
	return 0;
}

bool App::onInit() {
	return initSDL();
}

bool App::initSDL(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	if (!info) return false;
	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
		width = info->current_w;
		height = info->current_h;
	}
	screen = SDL_SetVideoMode(width, height, info->vfmt->BitsPerPixel, flags);
	if (!screen) return false;
	else return true;
}

void App::onLoop(){
}
void App::onRender(){
}
void App::onCleanup(){
	SDL_Quit();
	if (handler) {
		delete handler;
		handler = NULL;
		for (size_t i = 0; i < handlers.size(); ++i) delete handlers[i];
	}
}