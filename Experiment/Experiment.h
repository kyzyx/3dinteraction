#pragma once

#include <string>
#include "Scene.h"
#include "InputInterface.h"
#include "JSONLog.h"
//
// EXPERIMENT
//
// An Experiment is defined by an input type and an output type.
// It provides a sequence of Scenes.
//
class Experiment {
public:
   Experiment (std::string configFile);
   ~Experiment (void);
   
   bool init (void);
   Scene* getNextScene (void);
   InputStatus getInput (void);
   void onLoop (void);

private:
	InputInterface *m_inputDevice;
	int m_sceneIdx;
	int m_numScenes;
	Scene *m_curScene;
	JSONLog *m_log;
};