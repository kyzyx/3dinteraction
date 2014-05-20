#pragma once
#include "relativeinputinterface.h"
class HydraRelativeInterface :
	public RelativeInputInterface
{
public:
	HydraRelativeInterface(void);
	~HydraRelativeInterface(void);
protected:
	virtual InputStatus updateDelta();
	Eigen::Vector3d originpos;
	Eigen::Quaterniond originrot;
};

