#pragma once
#include "scene.h"
class TestScene :
	public Scene
{
public:
	TestScene(JSONLog *log);
	~TestScene(void);

protected:
	virtual void _processInput (InputStatus &input, InputStatus &deltaInput);
	virtual bool initMeshes(void);

	enum SceneState {
		WAIT_START,
		WAIT_TARGET,
		FINISHED
	};

	SceneState m_state;
};

