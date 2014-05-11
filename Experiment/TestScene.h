#pragma once
#include "scene.h"
class TestScene :
	public Scene
{
public:
	TestScene(void);
	~TestScene(void);

	virtual void processInput (InputStatus &input, InputStatus &deltaInput);
protected:
	virtual bool initMeshes(void);
};

