#include "Scene.h"
#include "D3DMesh.h"

Scene::Scene (void) {
	return;
}

Scene::~Scene (void) {
	for (MeshVec::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		delete (*it);
	}
}

bool Scene::init (D3DRenderer *render) {
	D3DMesh *mesh = new D3DMesh("ico.off", render);
	mesh->setColor(1,0,0);
	mesh->setTranslation(0,0,5);
	mesh->setScale(.2, .2, .2);
	m_meshes.push_back(mesh);

	mesh = new D3DMesh("ico.off", render);
	mesh->setColor(1,0,0);
	mesh->setTranslation(4,-3,5);
	mesh->setScale(.2, .2, .2);
	m_meshes.push_back(mesh);

	return true;
}

const Scene::MeshVec& Scene::getMeshes (void) {
	return m_meshes;
}

bool Scene::finished (void) {
	return m_finished;
}

void Scene::processInput (const InputStatus &input, const InputStatus &deltaInput) {
	return;
}
