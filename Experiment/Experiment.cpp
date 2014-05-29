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
#include "AdjustableInterface.h"
#include "TransformedInterface.h"
#include "SmoothedInterface.h"

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
		Eigen::Matrix4d m;
		m <<
			-0.99957198, -0.02474514,  0.01560538,  -0.730564,
             0.02044090, -0.97236698, -0.23256102,  24.426980,
			 0.02092891, -0.23214250,  0.97245660,  6.051621,
             0.        , 0.         ,  0.        ,  1.;
		m_inputDevice = new TransformedInterface(new ARInputInterface(), Xform(m));
    }
	else  if (input == "leap") {
		Eigen::Matrix4d m1, m2;
		m1 <<
		    9.97721366e-01,  -4.08118719e-02,   5.37258553e-02,     0.4551766,
			4.07157079e-02,   9.99166611e-01,   2.88367682e-03,   -36.1866713 + 1.15,
		   -5.37987690e-02,  -6.89619746e-04,   9.98551559e-01,    21.3061088,
		    0             ,   0             ,   0             ,    1;
		m2 <<
			 1.17, -0.0653715, -0.00466023, 0.0,
		    -0.0444751, 1.35, -0.161685, 10.0,
		     0.0, 0.0, 1.23, -4.0,
		     0, 0, 0, 1;
		m_inputDevice = new TransformedInterface(new TransformedInterface(new LeapInterface(), Xform(m1)), Xform(m2));
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
	else if (output == "2dprojections") {
		outputtype = OUTPUT_PROJECTIONS;
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

void Experiment::addAdjustable(App* app) {
	AdjustableInterface* ai = new AdjustableInterface(m_inputDevice, app);
	m_inputDevice = ai;
	app->addHandler(ai);
}

Scene* Experiment::getNextScene (void) {
	if (m_sceneIdx == m_numScenes) {
		return nullptr;
	}

	delete m_curScene;
	m_curScene = new TestScene(m_log);
	++m_sceneIdx;
	return m_curScene;
}

Eigen::Vector3d vzero() { return Eigen::Vector3d::Zero(); }

bool Experiment::init (D3DRenderer* r) {
	renderer = r;
	if (outputtype & OUTPUT_HEADTRACKED) {
		Eigen::Matrix4d m;
		// Insert camera->screen transformation here
		m <<
			-0.99957198, -0.02474514,  0.01560538,  -0.730564,
             0.02044090, -0.97236698, -0.23256102,  24.426980,
			 0.02092891, -0.23214250,  0.97245660,  6.051621,
             0.        , 0.         ,  0.        ,  1.;
		m_headtrackInput = new SmoothedInterface(
			new TransformedInterface(new ARInputInterface(), Xform(m)),
			new DoubleAvgFilter<Eigen::Vector3d, vzero>(12)
		);
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
	if (!(outputtype & OUTPUT_3D)) {
		renderer->disableStereo();
	}
	if ((outputtype & OUTPUT_HEADTRACKED) && m_headtrackInput != nullptr) {
		InputStatus arInput = m_headtrackInput->getStatus();
		// Fiducial -> eye position conversion
		arInput.pos += arInput.rot*Eigen::Vector3d(-5.15,0,-3);
		std::wstringstream msg;
		//msg	<< L"Tag Id: " << arInput.flags
		//	<< L", pos=("
		//	<< arInput.x() << L", "
		//	<< arInput.y() << L", "
		//	<< arInput.z()
		//	<< L")";
		//renderer->drawText(msg.str().c_str(), 300, 100, 0xffffffff, 20);
		msg.clear();
		msg << "Scene " << m_sceneIdx << " / " << m_numScenes;
		renderer->drawText(msg.str().c_str(), 300, 100, 0xffffffff, 20);
		((D3DRenderer*) renderer)->setHeadPosition(arInput.x(), arInput.y(), arInput.z(), 0.0311f);
	}
}
