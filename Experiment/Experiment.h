#pragma once

#include <string>
#include "Scene.h"
#include "InputInterface.h"

//
// EXPERIMENT
//
// An Experiment is defined by an input type and an output type.
// It provides a sequence of Scenes
//
class Experiment {
public:
   Experiment (std::string configFile);
   
   bool init (void);
   Scene getNextScene (void);
   InputStatus getInput (void);

private:
	InputInterface m_inputDevice;
};