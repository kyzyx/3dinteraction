#pragma once
#include "directxapp.h"
#include "Mesh.h"
#include "InputInterface.h"
#include "Experiment.h"
#include "Scene.h"

class ExperimentApp :
	public DirectXApp
{
public:
	ExperimentApp(void);
	virtual ~ExperimentApp(void);

	void onRender();
	void onLoop();
	bool onInit();

private:
	void drawMeshes();
	void setCamera(int camera);
	enum {CAMERA_MAIN, CAMERA_FRONT, CAMERA_TOP, CAMERA_SIDE};
	InputStatus lastInput;
	Experiment experiment;
	Scene *scene;

	D3DWindowTarget* views[4];
};

