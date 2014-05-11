#include "TestScene.h"
#include "InteractionSpace.h"

TestScene::TestScene(void)
{
}


TestScene::~TestScene(void)
{
}

bool TestScene::initMeshes() {
	Mesh *mesh;

	mesh = addMesh("target", "ico.off");
	mesh->setColor(1,0,0);
	mesh->setTranslation(-2,0,5);
	mesh->setScale(0.2f, 0.2f, 0.2f);

	mesh = addMesh("start", "ico.off");
	mesh->setColor(.4, .4, .4);
	mesh->setTranslation(2,0,5);
	mesh->setScale(0.2f, 0.2f, 0.2f);

	mesh = addMesh("spaceship", "spaceship.off");
	mesh->setColor(1,0,0);
	mesh->setTranslation(2,-1,7);
	mesh->setScale(0.2f, 0.2f, 0.2f);

	return true;
}

void TestScene::processInput (InputStatus &input, InputStatus &deltaInput) {
	double x = -deltaInput.timestamp * deltaInput.x() * 100;
	double y = -deltaInput.timestamp * deltaInput.y() * 100;
	double z = -deltaInput.timestamp * deltaInput.z() * 100;

	Mesh *ship = m_meshNames["spaceship"];
	Mesh *port = m_meshNames["start"];

	float d = (ship->getTranslation() - port->getTranslation()).norm();
	if (d < .2) {
		port->setColor(1,1,1);
	} else {
		port->setColor(.4,.4,.4);
	}

	ship->translateBy((float)x, (float)y, (float)z);
	ship->setTranslation(InteractionSpace::closestPointInVolume(ship->getTranslation()));
	ship->setRotation(input.rot.cast<float>());
}

