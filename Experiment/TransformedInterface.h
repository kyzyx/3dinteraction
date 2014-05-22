#pragma once
#include "InputInterface.h"

typedef Eigen::Transform<double, 3, Eigen::Affine> Xform;

class TransformedInterface : public InputInterface {
public:
	TransformedInterface(InputInterface* inputinterface, Xform xform) 
		: input(inputinterface), transform(xform), rot(xform.linear()) { ; }
	~TransformedInterface() { delete input; }

private:
	TransformedInterface() {;}
	virtual InputStatus _getStatus (void) {
		status = input->getStatus();
		status.pos = transform*status.pos;
		status.rot = rot*status.rot;
		
		return status;
	}

	Eigen::Transform<double,3,Eigen::Affine> transform;
	Eigen::Quaterniond rot;
	InputInterface* input;
};