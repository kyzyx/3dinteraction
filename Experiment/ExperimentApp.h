#pragma once
#include "directxapp.h"
#include "Mesh.h"
#include "InputInterface.h"
#include "Experiment.h"
#include "Scene.h"
#include "TrackLabel.h"

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
	InputStatus lastInput;
	Experiment experiment;
	Scene *scene;
	TrackLabel tracker;
};

