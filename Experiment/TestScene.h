#pragma once
#include "scene.h"
class TestScene :
	public Scene
{
public:
	TestScene(void);
	~TestScene(void);

protected:
	virtual void _processInput (InputStatus &input, InputStatus &deltaInput);
	virtual bool initMeshes(void);
};

