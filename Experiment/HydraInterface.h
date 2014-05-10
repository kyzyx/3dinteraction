#pragma once
#include "InputInterface.h"

class HydraInterface :
	public InputInterface
{
public:
	HydraInterface();
	~HydraInterface();

	virtual void update();
};