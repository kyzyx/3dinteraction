#pragma once
#include "Experiment.h"
#include "Alignment.h"
class CalibrationExperiment :
	public Experiment
{
public:
	CalibrationExperiment(void);
	~CalibrationExperiment(void);

	virtual Scene* getNextScene(void);
	void writeTransform(void);

protected:
	Alignment alignment;
};

