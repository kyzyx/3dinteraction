#include <random>

#include "TestScene.h"
#include "InteractionSpace.h"
#include "D3DMesh.h"

using namespace Eigen;
TestScene::TestScene(JSONLog *log) : Scene(), m_state(WAIT_START) {
	m_log = log;

	m_startPos = InputStatus();
	m_startPos.pos = InteractionSpace::randomPointInVolume().cast<double>();
	m_endPos = InputStatus();
	m_endPos.pos = InteractionSpace::randomPointInVolume().cast<double>();
	m_log->startScene(m_startPos, m_endPos);
}


TestScene::~TestScene(void)
{
}

double randd(){
	return rand()/(double) RAND_MAX;
}
bool TestScene::initMeshes() {
	Mesh *mesh;

	Eigen::Quaternionf rot = AngleAxisf(randd()*M_PI*0.5 - M_PI*0.25, Vector3f::UnitZ()) *
        AngleAxisf(randd()*M_PI*0.8 - M_PI*0.4,  Vector3f::UnitY()) *
	    AngleAxisf(2*M_PI/3 + randd()*M_PI*0.25, Vector3f::UnitX());
	mesh = addMesh("start", "spaceship.off", true, true);
	mesh->setColor(.4f, .4f, .4f);
	mesh->setRotation(rot);
	mesh->setTranslation(m_startPos.x(), m_startPos.y(), m_startPos.z());
	mesh->setScale(0.4f,0.4f,0.1f);

	rot = AngleAxisf(randd()*M_PI*0.5 - M_PI*0.25, Vector3f::UnitZ()) *
        AngleAxisf(randd()*M_PI*0.8 - M_PI*0.4,  Vector3f::UnitY()) *
	    AngleAxisf(2*M_PI/3 + randd()*M_PI*0.25, Vector3f::UnitX());
	mesh = addMesh("target", "spaceship.off", true, true);
	mesh->setColor(.4f,.4f,.4f);
	mesh->setTranslation(m_endPos.x(), m_endPos.y(), m_endPos.z());
	mesh->setRotation(rot);
	mesh->setScale(0.4f,0.4f,0.1f);

	mesh = addMesh("spaceship", "ico.off");
	//mesh->setColor(1,0,0);
	mesh->setTranslation(0,0,20);
	mesh->setScale(0.8f, 0.8f, 0.8f);

	return true;
}

void TestScene::_processInput (InputStatus &input, InputStatus &deltaInput) {
	if (input.inputType == InputStatus::NONE) {
		// Ignore invalid inputs
		return;
	}

	Mesh *ship = m_meshNames["spaceship"];
	Mesh *port; // where the spaceship wants to go

    // Choose the port	
	switch (m_state) {
	case WAIT_START:  port = m_meshNames["start"];  break;
	case WAIT_TARGET: port = m_meshNames["target"]; break;
	case FINISHED:    port = m_meshNames["target"]; break;
	default:          port = m_meshNames["start"];  break;
	}

	// Calculate useful values
	const float CLICK_DIST = 1.0f;
	float d = (ship->getTranslation() - port->getTranslation()).norm(); // distance to target
	bool validClick = d <= CLICK_DIST && input.isFlagSet(InputStatus::INPUTFLAG_SELECT);

	// Update colors based on conditions
	if (d <= CLICK_DIST) {
		port->setColor(1,1,1);
	} else {
		port->setColor(0.8f,0,0);
	}

	//if (input.isFlagSet(InputStatus::INPUTFLAG_SELECT)) {
	//	ship->setColor(0,0,1);
	//} else {
	//	ship->setColor(1,0,0);
	//}

	// Update the state machine
	switch (m_state) {
	case WAIT_START:
		if (validClick) {
			m_started = true;
			port->setColor(.4f, .4f, .4f);
			m_state = WAIT_TARGET;
		}
		break;
	case WAIT_TARGET:
		if (validClick) {
			m_started = false;
			m_state = FINISHED;
		}
		break;
	case FINISHED:
		port->setColor(0,.5f,.5f);
		m_finished = true;		
		break;
	default:
		break;
	}

	// Update the ship's position
	ship->setTranslation((float)input.x(), (float)input.y(), (float)input.z());
	if (input.inputType != InputStatus::HYDRA && input.inputType != InputStatus::LEAP) {
		ship->setTranslation(InteractionSpace::closestPointInVolume(ship->getTranslation(), input.inputType != InputStatus::ARTAG));
	}
	if (input.isFlagSet(InputStatus::INPUTFLAG_ACTIVE)) ship->setColor(1.f, 0.f, 0.f);
	else ship->setColor(0.7f,0.f,1.f);
	ship->setRotation(input.rot.cast<float>());
}
