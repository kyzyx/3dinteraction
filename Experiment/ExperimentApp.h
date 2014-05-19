#pragma once
#include "directxapp.h"
#include "Mesh.h"
#include "InputInterface.h"
#include "Experiment.h"
#include "Scene.h"
#include "ARInterface.h"

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
	ARInterface *m_arInput;
	Scene *scene;
};

