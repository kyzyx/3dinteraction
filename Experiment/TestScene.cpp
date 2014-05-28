#include <random>

#include "TestScene.h"
#include "InteractionSpace.h"
#include "D3DMesh.h"

TestScene::TestScene(JSONLog *log) : Scene(), m_state(WAIT_START) {
	m_log = log;

	std::random_device rd;
	std::default_random_engine el(rd());
	std::uniform_real_distribution<float> x_uniform_dist(-15,15);
	std::uniform_real_distribution<float> y_uniform_dist(-8,8);
	std::uniform_real_distribution<float> z_uniform_dist(10,20);

	m_startPos = InputStatus();
	m_startPos.x() = x_uniform_dist(el);
	m_startPos.y() = y_uniform_dist(el);
	m_startPos.z() = z_uniform_dist(el);
	m_endPos = InputStatus();
	m_endPos.x() = x_uniform_dist(el);
	m_endPos.y() = y_uniform_dist(el);
	m_endPos.z() = z_uniform_dist(el);
	m_log->startScene(m_startPos, m_endPos);
}


TestScene::~TestScene(void)
{
}

bool TestScene::initMeshes() {
	Mesh *mesh;

	mesh = addMesh("start", "ico.off");
	mesh->setColor(.4f, .4f, .4f);
	mesh->setTranslation(m_startPos.x(), m_startPos.y(), m_startPos.z());
	mesh->setScale(1,1,1);

	mesh = addMesh("target", "ico.off");
	mesh->setColor(.4f,.4f,.4f);
	mesh->setTranslation(m_endPos.x(), m_endPos.y(), m_endPos.z());
	mesh->setScale(1,1,1);

	mesh = addMesh("spaceship", "pointer.off", true, true);
	//mesh->setColor(1,0,0);
	mesh->setTranslation(0,0,20);
	mesh->setScale(0.4f, 0.4f, 0.1f);

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
	const float CLICK_DIST = 0.8f;
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
	ship->setTranslation(InteractionSpace::closestPointInVolume(ship->getTranslation(), input.inputType != InputStatus::ARTAG));
	ship->setRotation(input.rot.cast<float>());
}
