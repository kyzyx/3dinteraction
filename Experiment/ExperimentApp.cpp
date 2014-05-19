#include <cstdio>
#include <cmath>
#include <sstream>
#include "ExperimentApp.h"
#include "D3DMesh.h"
#include "Experiment.h"
#include "Scene.h"

ExperimentApp::ExperimentApp(void) : DirectXApp(false), experiment("exp1.json"), m_arInput(nullptr)
{
}


ExperimentApp::~ExperimentApp(void)
{
	if (m_arInput != nullptr) {
		delete m_arInput;
	}
}

bool ExperimentApp::onInit(void) {
	if (!DirectXApp::onInit()) return false;

	//m_arInput = new ARInterface();

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

	int line = 0;
	InputStatus arInput = experiment.getInput();//m_arInput->getTag(10);
	std::wstringstream msg;
	msg
		<< L"Id: " << arInput.flags
		<< L", pos=("
		<< arInput.pos.x() << L", "
		<< arInput.pos.y() << L", "
		<< arInput.pos.z()
		<< L")";
	render->drawText(msg.str().c_str(), 300, 100+30*line, 0xffffffff, 20);
	++line;

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
