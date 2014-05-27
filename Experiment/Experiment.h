#pragma once

#include <string>
#include "Scene.h"
#include "InputInterface.h"
#include "JSONLog.h"
#include "D3DRenderer.h"

//
// EXPERIMENT
//
// An Experiment is defined by an input type and an output type.
// It provides a sequence of Scenes.
//
class Experiment {
public:
   enum {OUTPUT_2D = 0, OUTPUT_3D = 1, OUTPUT_HEADTRACKED = 2, OUTPUT_PROJECTIONS = 4};
	Experiment (std::string configFile);
   ~Experiment (void);
   
   bool init (D3DRenderer* renderer);
   Scene* getNextScene (void);
   InputStatus getInput (void);
   void onLoop (void);

   int getOutputType(void) const { return outputtype; }

   void addAdjustable(App* app);
private:
	D3DRenderer* renderer;
	InputInterface *m_headtrackInput;
	InputInterface *m_inputDevice;
	int m_sceneIdx;
	int m_numScenes;
	int outputtype;
	Scene *m_curScene;
	JSONLog *m_log;
};