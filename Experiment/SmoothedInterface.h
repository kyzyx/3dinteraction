#pragma once
#include "inputinterface.h"
#include "Filter.h"
class SmoothedInterface :
	public InputInterface
{
public:
	SmoothedInterface(InputInterface* inputinterface, Filter<Eigen::Vector3d>* filter);
	~SmoothedInterface(void);
	virtual void update() { input->update(); }
protected:
	SmoothedInterface(void) {;}
	virtual InputStatus _getStatus(void);

	Filter<Eigen::Vector3d>* f;
	InputInterface* input;

};

