#pragma once
#include "directxapp.h"
#include "Mesh.h"

class ExperimentApp :
	public DirectXApp
{
public:
	ExperimentApp(void);
	~ExperimentApp(void);

	void onRender();
	void onLoop();
	bool onInit();

private:
	Mesh* mesh;
};

