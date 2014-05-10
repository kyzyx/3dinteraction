#include "Scene.h"
#include "D3DMesh.h"
#include "math.h"

Scene::Scene (void) : m_finished(false) {
	return;
}

Scene::~Scene (void) {
	for (MeshVec::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		delete (*it);
	}
}

bool Scene::init (D3DRenderer *renderer) {
	m_renderer = renderer;

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

const Scene::MeshVec& Scene::getMeshes (void) {
	return m_meshes;
}

bool Scene::finished (void) {
	return m_finished;
}

float dist (float *a, float *b) {
	double dx = a[0] - b[0];
	double dy = a[1] - b[1];
	double dz = a[2] - b[2];
	return sqrt(dx*dx + dy*dy + dz*dz);
}

void Scene::processInput (InputStatus &input, InputStatus &deltaInput) {
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

	Eigen::Quaterniond q = input.rot;
	double roll = atan2(2.0*q.y()*q.w() - 2.0*q.x()*q.z(), 1 - 2.0*q.y()*q.y() - 2.0*q.z()*q.z());
	double pitch = atan2(2.0*q.x()*q.w() - 2.0*q.y()*q.z(), 1 - 2.0*q.x()*q.x() - 2.0*q.z()*q.z());
	double yaw = asin(2.0*q.x()*q.y() + 2.0*q.z()*q.w());
	ship->setRotation(roll, pitch, yaw);

	return;
}

Mesh* Scene::addMesh (std::string meshName, std::string filename) {
	// Overwrite any current meshes with the same name
	if (m_meshNames.count(meshName) != 0) {
		removeMesh(meshName);
	}

	Mesh *mesh = new D3DMesh(filename.c_str(), m_renderer);
	m_meshes.push_back(mesh);
	m_meshNames[meshName] = mesh;
	return mesh;
}

void Scene::removeMesh (std::string meshName) {
	Mesh *mesh = m_meshNames[meshName];
	m_meshes.remove(mesh);
	m_meshNames.erase(meshName);
	delete mesh;
}
