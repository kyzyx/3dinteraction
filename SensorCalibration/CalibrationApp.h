#pragma once
#include "Mesh.h"
#include "InputInterface.h"
#include "CalibrationExperiment.h"
#include "Scene.h"
#include "ExperimentApp.h"

class CalibrationApp :
	public ExperimentApp
{
public:
	CalibrationApp(void);
	~CalibrationApp(void);
	
	void onLoop();
};

