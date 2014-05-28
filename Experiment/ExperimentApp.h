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
	ExperimentApp(std::string configfile);
	virtual ~ExperimentApp(void);

	void onRender();
	void onLoop();
	bool onInit();

private:
	void drawMeshes();
	void setCamera(int camera);
	enum {CAMERA_MAIN, CAMERA_FRONT, CAMERA_TOP, CAMERA_SIDE};
	D3DWindowTarget* views[4];
protected:
	Experiment* experiment;
	Scene *scene;
};

