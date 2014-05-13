#pragma once
#include "directxapp.h"

#include "Mesh.h"
#include "InputInterface.h"
#include "CalibrationExperiment.h"
#include "Scene.h"

class CalibrationApp :
	public DirectXApp
{
public:
	CalibrationApp(void);
	~CalibrationApp(void);
	
	void onRender();
	void onLoop();
	bool onInit();

private:
	InputStatus lastInput;
	CalibrationExperiment experiment;
	Scene *scene;
};

