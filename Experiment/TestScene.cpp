#include "TestScene.h"
#include "InteractionSpace.h"

TestScene::TestScene(JSONLog *log) : Scene(log), m_state(WAIT_START)
{
}


TestScene::~TestScene(void)
{
}

bool TestScene::initMeshes() {
	Mesh *mesh;

	mesh = addMesh("target", "ico.off");
	mesh->setColor(1,0,0);
	mesh->setTranslation(-10,0,15);
	mesh->setScale(2,2,2);

	mesh = addMesh("start", "ico.off");
	mesh->setColor(.4, .4, .4);
	mesh->setTranslation(10,0,15);
	mesh->setScale(2,2,2);

	mesh = addMesh("spaceship", "spaceship.off");
	mesh->setColor(1,0,0);
	mesh->setTranslation(15,-5,20);
	mesh->setScale(0.2f, 0.2f, 0.05f);

	return true;
}

void TestScene::_processInput (InputStatus &input, InputStatus &deltaInput) {
	double x = -deltaInput.timestamp * deltaInput.x() * 100;
	double y = -deltaInput.timestamp * deltaInput.y() * 100;
	double z = -deltaInput.timestamp * deltaInput.z() * 100;

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
	float d = (ship->getTranslation() - port->getTranslation()).norm(); // distance to target
	bool validClick = d < 0.2 && input.flagSet(InputStatus::INPUTFLAG_SELECT);

	// Update colors based on conditions
	if (d < .2) {
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
		// TODO end it all
		break;
	default:
		break;
	}

	// Update the ship's position
	ship->translateBy((float)x, (float)y, (float)z);
	ship->setTranslation(InteractionSpace::closestPointInVolume(ship->getTranslation()));
	ship->setRotation(input.rot.cast<float>());
}

