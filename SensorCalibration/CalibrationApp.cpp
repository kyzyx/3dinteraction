#include "CalibrationApp.h"

CalibrationApp::CalibrationApp(void) : DirectXApp(false)
{
}


CalibrationApp::~CalibrationApp(void)
{
}


bool CalibrationApp::onInit(void) {
	if (!DirectXApp::onInit()) return false;

	experiment.init();
    scene = experiment.getNextScene();	
	scene->init((D3DRenderer*)render);

	return true;
}

void CalibrationApp::onRender(void) {
	float eye[] = {0.f,0.f,80.f};
	float towards[] = {0.f,0.f,-1.f};
	float up[] = {0.f,1.f,0.f};
	render->clear();
	float hfov = 41;
	float aspectratio = 16.f/9.f;
	render->setProjection(hfov, aspectratio);
	render->lookAt(eye, towards, up);

	if (scene) {
		const Scene::MeshVec &meshes = scene->getMeshes();
		for (Scene::MeshVec::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
			(*it)->draw();
		}
	}

	render->display();
}

void CalibrationApp::onLoop(void) {
    experiment.onLoop();
	if (scene->finished()) {
		OutputDebugString("Next scene!\n");
		scene = experiment.getNextScene();
		if (scene) scene->init((D3DRenderer*)render);
		else {
			running = false;
			experiment.writeTransform();
		}
	} else {
		InputStatus input = experiment.getInput();
		InputStatus deltaInput = input - lastInput;
		lastInput = input;
		scene->processInput(input, deltaInput);
	}
}
