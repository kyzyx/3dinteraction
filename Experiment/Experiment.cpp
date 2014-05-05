#include <fstream>
#include "Experiment.h"
#include "json.h"
#include "InputInterface.h"
#include "MouseKeyboardInterface.h"

Experiment::Experiment (std::string configFile) :
m_inputDevice(NULL), m_sceneIdx(0), m_curScene(NULL) {
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
	return;
}

Experiment::~Experiment (void) {
	delete m_inputDevice;
	delete m_curScene;
}

Scene* Experiment::getNextScene (void) {
	if (m_sceneIdx == m_numScenes) {
		return NULL;
	}

	delete m_curScene;
	m_curScene = new Scene();
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