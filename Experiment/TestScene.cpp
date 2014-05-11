#include "TestScene.h"


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

float dist (float *a, float *b) {
	double dx = a[0] - b[0];
	double dy = a[1] - b[1];
	double dz = a[2] - b[2];
	return sqrt(dx*dx + dy*dy + dz*dz);
}

void TestScene::processInput (InputStatus &input, InputStatus &deltaInput) {
	double x = -deltaInput.timestamp * deltaInput.x() * 100;
	double y = -deltaInput.timestamp * deltaInput.y() * 100;
	double z = -deltaInput.timestamp * deltaInput.z() * 100;

	Mesh *ship = m_meshNames["spaceship"];
	Mesh *port = m_meshNames["start"];

	float shipPos[3];
	float portPos[3];
	ship->getTranslation(shipPos);
	port->getTranslation(portPos);
	float d = dist(shipPos, portPos);

	if (d < .2) {
		port->setColor(1,1,1);
	} else {
		port->setColor(.4,.4,.4);
	}

	ship->translateBy((float)x, (float)y, (float)z);
	float t[3];
    ship->getTranslation(t);
	if (t[0] < -3) t[0] = -3;
	if (t[0] >  3) t[0] =  3;
	if (t[1] < -2) t[1] = -2;
	if (t[1] >  2) t[1] =  2;
	if (t[2] <  3) t[2] =  3;
	if (t[2] >  7) t[2] =  7;
    ship->setTranslation(t);

	ship->setRotation(input.rot.cast<float>());

	return;
}

