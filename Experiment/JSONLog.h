#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <array>

#include "InputStatus.h"
#include "json.h"

class JSONLog {
public:
	typedef std::array<float,9> RawInput;
	JSONLog(std::string logName);
	~JSONLog(void);

	void startExperiment(json::Value config);
	void endExperiment();

	void startScene();
	void endScene();

	void logInput(InputStatus &input);

private:
	json::Object& getSceneObj(); // return reference to the current scene

	std::ofstream m_logFile;
	int m_sceneCounter;
	json::Object m_object;
	std::vector<RawInput> inputs;
};

