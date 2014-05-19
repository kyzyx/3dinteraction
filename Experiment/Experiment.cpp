#include <fstream>
#include <sstream>
#include <ctime>
#include "Experiment.h"
#include "json.h"
#include "InputInterface.h"
#include "MouseKeyboardInterface.h"
#include "HydraInterface.h"
#include "TestScene.h"

Experiment::Experiment (std::string configFile) :
m_inputDevice(nullptr), m_sceneIdx(0), m_curScene(nullptr) {
	std::ifstream inFile(configFile);
	std::string jsonString( 
		(std::istreambuf_iterator<char>(inFile) ),
		(std::istreambuf_iterator<char>()       ));
	json::Value config = json::Deserialize(jsonString);
	std::string subject = config["subject"].ToString();
	std::string input = config["input"].ToString();
	std::string output = config["output"].ToString();
	m_numScenes = config["scenes"].ToInt();

	if (input == "mousekbd") {
		m_inputDevice = new MouseKeyboardInterface();
	}
	else if (input == "hydra") {
		m_inputDevice = new HydraInterface();
	}

	// Figure out what the log name should be and create it
	std::time_t t = std::time(0); // get NOW
	struct std::tm *now = std::localtime(&t);
	std::stringstream logName;
	logName << subject << "_" 
		<< (now->tm_year + 1900) << "_"
		<< (now->tm_mon + 1) << "_"
		<< (now->tm_mday) << "_"
		<< (now->tm_hour) << "_"
		<< (now->tm_min) << "_"
		<< (now->tm_sec);
	m_log = new JSONLog(logName.str());

	// Start logging data
	m_log->startExperiment(config);
	
	return;
}

Experiment::~Experiment (void) {
	delete m_inputDevice;
	delete m_curScene;
	m_log->endExperiment();
	delete m_log;
}

Scene* Experiment::getNextScene (void) {
	if (m_sceneIdx == m_numScenes) {
		return NULL;
	}

	delete m_curScene;
	m_curScene = new TestScene(m_log);
	++m_sceneIdx;
	return m_curScene;
}


bool Experiment::init (void) {
	return true;
}

InputStatus Experiment::getInput (void) {
	return m_inputDevice->getStatus();
}

void Experiment::onLoop (void) {
	m_inputDevice->update();
}