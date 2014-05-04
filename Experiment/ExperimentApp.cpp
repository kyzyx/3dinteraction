#include <cstdio>
#include <cmath>
#include "ExperimentApp.h"
#include "D3DMesh.h"
#include "InputInterface.h"
#include "MouseKeyboardInterface.h"
#include "InputHandler.h"
#include "Experiment.h"
#include "Scene.h"

ExperimentApp::ExperimentApp(void) : DirectXApp(false), experiment("exp1.json")
{
}


ExperimentApp::~ExperimentApp(void)
{
}

bool ExperimentApp::onInit(void) {
	if (!DirectXApp::onInit()) return false;

	InputInterface* input = new MouseKeyboardInterface();
	handler = new InputHandler(input);

	// PSEUDOCODE
	experiment.init();
    scene = experiment.getNextScene();	
	scene.init((D3DRenderer*)render);
	// END PSEUDOCODE

	return true;
}

float eye[] = {0.f,0.f,0.f};
float towards[] = {0.f,0.f,1.f};
float up[] = {0.f,1.f,0.f};

void ExperimentApp::onRender(void) {
	render->clear();
	float hfov = 45;
	float aspectratio = 4.f/3.f;
	render->setProjection(hfov, aspectratio);
	render->lookAt(eye, towards, up);

	const Scene::MeshVec &meshes = scene.getMeshes();
	for (Scene::MeshVec::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->draw();
	}

	// Uncomment to draw edges in black
	/*mesh->setColor(0,0,0);
	mesh->translateBy(0,0,0.0001);
	mesh->drawWireframe();
	mesh->translateBy(0,0,-0.0001);
	mesh->setColor(1,0,0);*/

	render->display();
}

void ExperimentApp::onLoop(void) {
	// PSEUDOCODE
	if (scene.finished()) {
		scene = experiment.getNextScene();
        scene.init((D3DRenderer*)render);
        meshes = scene.getMeshes();
	}

	InputStatus input = experiment.getInput();
	InputStatus deltaInput = input - lastInput;
	lastInput = input;
	scene.processInput(input, deltaInput);
	// END PSEUDOCODE
}