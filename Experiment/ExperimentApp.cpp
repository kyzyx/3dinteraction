#include "ExperimentApp.h"
#include "D3DMesh.h"

ExperimentApp::ExperimentApp(void) : DirectXApp(false)
{
}


ExperimentApp::~ExperimentApp(void)
{
}

bool ExperimentApp::onInit(void) {
	if (!DirectXApp::onInit()) return false;
	mesh = new D3DMesh("ico.off", (D3DRenderer*) render);
	mesh->setColor(1,0,0);
	mesh->setTranslation(0,0,5);
	return true;
}

float eye[] = {0.f,0.f,0.f};
float towards[] = {0.f,0.f,1.f};
float up[] = {0.f,1.f,0.f};

void ExperimentApp::onRender(void) {
	render->clear();
	float hfov = 45;
	float aspectratio = 4./3.;
	render->setProjection(hfov, aspectratio);
	render->lookAt(eye, towards, up);
	mesh->draw();

	// Uncomment to draw edges in black
	/*mesh->setColor(0,0,0);
	mesh->translateBy(0,0,0.0001);
	mesh->drawWireframe();
	mesh->translateBy(0,0,-0.0001);
	mesh->setColor(1,0,0);*/

	render->display();
}

void ExperimentApp::onLoop(void) {
}