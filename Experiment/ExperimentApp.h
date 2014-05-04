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
	~ExperimentApp(void);

	void onRender();
	void onLoop();
	bool onInit();

private:
	InputStatus lastInput;
	Experiment experiment;
	Scene scene;
};

