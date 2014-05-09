#include "Scene.h"
#include "D3DMesh.h"

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

void Scene::processInput (InputStatus &input, InputStatus &deltaInput) {
	double x = -deltaInput.timestamp * deltaInput.x() * 100;
	double y = -deltaInput.timestamp * deltaInput.y() * 100;
	double z = -deltaInput.timestamp * deltaInput.z() * 100;

	Mesh *ship = m_meshNames["spaceship"];
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