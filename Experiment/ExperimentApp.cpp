#include <cstdio>
#include <cmath>
#include <sstream>
#include "ExperimentApp.h"
#include "D3DMesh.h"
#include "Experiment.h"
#include "Scene.h"
#include "InteractionSpace.h"

ExperimentApp::ExperimentApp(void) : DirectXApp(false), experiment(nullptr)
{
}

ExperimentApp::ExperimentApp(std::string configfile) : DirectXApp(false), experiment(new Experiment(configfile))
{
}


ExperimentApp::~ExperimentApp(void)
{
	if (experiment) delete experiment;
}

bool ExperimentApp::onInit(void) {
	if (!DirectXApp::onInit()) return false;

	D3DRenderer* dr = (D3DRenderer*) render;
	experiment->init(dr);
    scene = experiment->getNextScene();	
	scene->init(dr);
	experiment->addAdjustable(this);

	for (int i = 0; i < 4; ++i) {
		views[i] = dr->InitializeWindowTarget(width/2, height/2);
	}

	SDL_ShowCursor(SDL_DISABLE);
	return true;
}

void ExperimentApp::setCamera(int camera) {
	const float headdistance = 60.f;
	const float posx[] = {1.f,0.f,0.f};
	const float negz[] = {0.f,0.f,-1.f};
	const float posy[] = {0.f,1.f,0.f};
	const float negy[] = {0.f,-1.f,0.f};
	const float eye[] = {0.f,0.f,headdistance};
	const float sideeye[] = {-InteractionSpace::screenwidth(),0.f,headdistance/2 - 2};
	const float topeye[] = {0.f,InteractionSpace::screenheight(), headdistance/2 - 2};
	float hfov = 41;
	switch (camera) {
		case CAMERA_MAIN:
			render->setProjection(hfov, width/((float) height));
			render->lookAt(eye, negz, posy);
			break;
		case CAMERA_TOP:
			render->ortho(InteractionSpace::screenwidth(), headdistance);
			render->lookAt(topeye, negy, negz);
			break;
		case CAMERA_SIDE:
			render->ortho(headdistance, InteractionSpace::screenheight());
			render->lookAt(sideeye, posx, posy);
			break;
		case CAMERA_FRONT:
			render->ortho(InteractionSpace::screenwidth(), InteractionSpace::screenheight());
			render->lookAt(eye, negz, posy);
			break;
	}
}


void ExperimentApp::onRender(void) {
	if (!scene) return;
	D3DRenderer* dr = (D3DRenderer*) render;
	if (experiment->getOutputType() & Experiment::OUTPUT_PROJECTIONS) {
		for (int i = 0; i < 4; ++i) {
			dr->setRenderTarget(views[i]->rt);
			if (i&2) dr->setClearColor(0.f,0.f,0.f);
			else     dr->setClearColor(0.08f,0.08f,0.08f);
			dr->clearRenderTarget(views[i]->rt);
			if (i) dr->setAmbient(1.f,1.f,1.f);
			setCamera(i);
			drawMeshes();
		}
		dr->setAmbient(0.f,0.f,0.f);

		dr->setRenderTarget();
		dr->DrawWindowTarget(views[0], width/2, 0);
		dr->DrawWindowTarget(views[1], 0, height/2);
		dr->DrawWindowTarget(views[2], 0, 0);
		dr->DrawWindowTarget(views[3], width/2, height/2);
	} else {
		dr->setRenderTarget();
		setCamera(CAMERA_MAIN);
		drawMeshes();
	}

	render->display();
	render->clear();
}

void ExperimentApp::drawMeshes(void) {
	const Scene::MeshVec &meshes = scene->getMeshes();
	for (Scene::MeshVec::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
		(*it)->draw();
	}
}

void ExperimentApp::onLoop(void) {
	experiment->onLoop();
	if (scene->finished()) {
		scene = experiment->getNextScene();
        scene->init((D3DRenderer*)render);
	}

	InputStatus input = experiment->getInput();
	scene->processInput(input);
}
