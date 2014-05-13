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

	void startScene(const InputStatus &start, const InputStatus &end);
	void endScene();

	// Saves inputs at a frequency of 1/m_inputSavePeriod
	void logInput(InputStatus &input);

private:
	json::Object& getSceneObj(); // return reference to the current scene

	std::string m_basename;
	std::ofstream m_logFile;
	std::ofstream m_datFile;
	int m_sceneCounter;
	json::Object m_object;
	std::vector<RawInput> m_inputs;
	double m_inputLastTime;
	double m_inputSavePeriod; // in seconds
};

