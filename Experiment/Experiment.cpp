#include <fstream>
#include <sstream>
#include <ctime>
#include "Experiment.h"
#include "json.h"
#include "InputInterface.h"
#include "MouseKeyboardInterface.h"
#include "HydraInterface.h"
#include "HydraRelativeInterface.h"
#include "ARInterface.h"
#include "LeapInterface.h"
#include "TestScene.h"
#include "TransformedInterface.h"

Experiment::Experiment (std::string configFile) :
m_inputDevice(nullptr), m_sceneIdx(0), m_curScene(nullptr), m_headtrackInput(nullptr) {
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
		Eigen::Matrix4d m;
		// Insert hydra->screen transformation here
		m.setIdentity();
		m_inputDevice = new TransformedInterface(new HydraInterface(), Xform(m));
	}
	else if (input == "3dmouse") {
		m_inputDevice = new HydraRelativeInterface();
	}
	else if (input == "aruco") {
		m_inputDevice = new ARInputInterface();
    }
	else  if (input == "leap") {
		Eigen::Matrix4d m;
		// Insert leap->screen transformation here
		m.setIdentity();
		m_inputDevice = new TransformedInterface(new LeapInterface(), Xform(m));
	}

	if (output == "3d") {
		outputtype = OUTPUT_3D;
	}
	else if (output == "2d") {
		outputtype = OUTPUT_2D;
	}
	else if (output == "3dHeadtracked") {
		outputtype = OUTPUT_3D | OUTPUT_HEADTRACKED;
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
	if (m_headtrackInput) delete m_headtrackInput;
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


bool Experiment::init (D3DRenderer* r) {
	renderer = r;
	if (outputtype & OUTPUT_HEADTRACKED) {
		Eigen::Matrix4d m;
		// Insert camera->screen transformation here
		m.setIdentity();
		m_headtrackInput = new TransformedInterface(new ARInputInterface(), Xform(m));
		renderer->EnableHeadtracking();
		renderer->setHeadPosition(0.f,0.f,60.f,0.0311f);
	}
	if (outputtype & OUTPUT_3D) {
		renderer->enableStereo();
	} else {
		renderer->disableStereo();
	}
	return true;
}

InputStatus Experiment::getInput (void) {
	return m_inputDevice->getStatus();
}

void Experiment::onLoop (void) {
	m_inputDevice->update();
	if ((outputtype & OUTPUT_HEADTRACKED) && m_headtrackInput != nullptr) {
		InputStatus arInput = m_headtrackInput->getStatus();
		std::wstringstream msg;
		msg	<< L"Tag Id: " << arInput.flags
			<< L", pos=("
			<< arInput.x() << L", "
			<< arInput.y() << L", "
			<< arInput.z()
			<< L")";
		renderer->drawText(msg.str().c_str(), 300, 100, 0xffffffff, 20);
		((D3DRenderer*) renderer)->setHeadPosition(arInput.x(), arInput.y(), arInput.z(), 0.0311f);
	}
}
