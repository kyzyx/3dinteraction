#include <cstdio>
#include <cmath>
#include "ExperimentApp.h"
#include "D3DMesh.h"
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

	experiment.init();
    scene = experiment.getNextScene();	
	scene->init((D3DRenderer*)render);

	return true;
}

float eye[] = {0.f,0.f,80.f};
float towards[] = {0.f,0.f,-1.f};
float up[] = {0.f,1.f,0.f};

void ExperimentApp::onRender(void) {
	render->clear();
	float hfov = 41;
	float aspectratio = 16.f/9.f;
	render->setProjection(hfov, aspectratio);
	render->lookAt(eye, towards, up);

	const Scene::MeshVec &meshes = scene->getMeshes();
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
    experiment.onLoop();
	if (scene->finished()) {
		scene = experiment.getNextScene();
        scene->init((D3DRenderer*)render);
	}

	InputStatus input = experiment.getInput();
	scene->processInput(input);
}