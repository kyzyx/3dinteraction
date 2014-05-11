#include "Scene.h"
#include "D3DMesh.h"
#include "math.h"

Scene::Scene (void) : m_finished(false) {
}

Scene::~Scene (void) {
	for (MeshVec::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		delete (*it);
	}
}

bool Scene::init (D3DRenderer *renderer) {
	m_renderer = renderer;
	return initMeshes();
}

const Scene::MeshVec& Scene::getMeshes (void) {
	return m_meshes;
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
