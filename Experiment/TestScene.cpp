#include "TestScene.h"
#include "InteractionSpace.h"
#include "D3DMesh.h"

TestScene::TestScene(JSONLog *log) : Scene(log), m_state(WAIT_START)
{
}


TestScene::~TestScene(void)
{
}

bool TestScene::initMeshes() {
	Mesh *mesh;

	mesh = addMesh("target", "ico.off");
	mesh->setColor(.4,.4,.4);
	mesh->setTranslation(-10,0,15);
	mesh->setScale(1,1,1);

	mesh = addMesh("start", "ico.off");
	mesh->setColor(.4, .4, .4);
	mesh->setTranslation(10,0,15);
	mesh->setScale(1,1,1);

	Mesh* m = new D3DMesh("spaceship.off", m_renderer, true, true);
	mesh = addMesh("spaceship", m);
	mesh->setColor(1,0,0);
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
	const float CLICK_DIST = 0.8;
	float d = (ship->getTranslation() - port->getTranslation()).norm(); // distance to target
	bool validClick = d <= CLICK_DIST && input.flagSet(InputStatus::INPUTFLAG_SELECT);

	// Update colors based on conditions
	if (d <= CLICK_DIST) {
		port->setColor(1,1,1);
	} else {
		port->setColor(0.8,0,0);
	}

	if (input.flagSet(InputStatus::INPUTFLAG_SELECT)) {
		ship->setColor(0,0,1);
	} else {
		ship->setColor(1,0,0);
	}

	// Update the state machine
	switch (m_state) {
	case WAIT_START:
		if (validClick) {
			m_started = true;
			port->setColor(.4, .4, .4);
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
		port->setColor(0,.5,.5);
		// TODO end it all
		break;
	default:
		break;
	}

	// Update the ship's position
	ship->setTranslation(input.x(), input.y(), input.z());
	ship->setTranslation(InteractionSpace::closestPointInVolume(ship->getTranslation(), input.inputType != InputStatus::ARTAG));
	ship->setRotation(input.rot.cast<float>());
}
