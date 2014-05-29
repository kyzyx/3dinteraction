#include <random>

#include "TestScene.h"
#include "InteractionSpace.h"
#include "D3DMesh.h"
#include <ctime>

using namespace Eigen;

double randd(){
  static int first = 1;
  if (first) {
    srand(time(0));
    first = 0;
  }

  // Return random number
  int r1 = rand();
  double r2 = ((double) rand()) / ((double) RAND_MAX);
  return (r1 + r2) / ((double) RAND_MAX);
}

TestScene::TestScene(JSONLog *log) : Scene(), m_state(WAIT_START) {
	m_log = log;

	Vector3d sphereCenter(0,0,15);
	double sphereradius = 15;

	double z = sphereradius*(1.25*randd() - .625);
    double at = 0.54;
	double phi = (M_PI - 2*at)*randd() + at;
	if (randd() < 0.5) phi = -phi;
	double d = sqrt(sphereradius*sphereradius - z*z);

	m_startPos = InputStatus();
	m_startPos.pos = sphereCenter + Vector3d(d*sin(phi), d*cos(phi), z);
	m_endPos = InputStatus();
	m_endPos.pos = sphereCenter + Vector3d(-d*sin(phi), -d*cos(phi), -z);
	m_log->startScene(m_startPos, m_endPos);
}


TestScene::~TestScene(void)
{
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
	mesh->setScale(0.5f,0.5f,0.125f);

	rot = AngleAxisf(randd()*M_PI*0.5 - M_PI*0.25, Vector3f::UnitZ()) *
        AngleAxisf(randd()*M_PI*0.8 - M_PI*0.4,  Vector3f::UnitY()) *
	    AngleAxisf(2*M_PI/3 + randd()*M_PI*0.25, Vector3f::UnitX());
	mesh = addMesh("target", "spaceship.off", true, true);
	mesh->setColor(.4f,.4f,.4f);
	mesh->setTranslation(m_endPos.x(), m_endPos.y(), m_endPos.z());
	mesh->setRotation(rot);
	mesh->setScale(0.5f,0.5f,0.125f);

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
	const float CLICK_DIST = 1.2f;
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
		//ship->setTranslation(InteractionSpace::closestPointInVolume(ship->getTranslation(), input.inputType != InputStatus::ARTAG));
	}
	if (input.isFlagSet(InputStatus::INPUTFLAG_ACTIVE)) ship->setColor(1.f, 0.f, 0.f);
	else ship->setColor(0.7f,0.f,1.f);
	ship->setRotation(input.rot.cast<float>());
}
