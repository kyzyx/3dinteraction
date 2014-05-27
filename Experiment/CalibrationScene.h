#pragma once
#include "scene.h"
#include "Alignment.h"

class CalibrationScene :
	public Scene
{
public:
	CalibrationScene(Alignment* a);
	CalibrationScene(Alignment* a, Eigen::Vector3f p);
	~CalibrationScene(void);

	virtual void processInput (InputStatus &input, InputStatus &deltaInput);
protected:
	virtual bool initMeshes(void);

	Alignment* alignment;
	Eigen::Vector3f pos;
};

