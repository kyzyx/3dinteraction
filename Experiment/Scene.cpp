#include "Scene.h"
#include "D3DMesh.h"
#include "math.h"

Scene::Scene (void) : m_finished(false), m_log(nullptr), m_started(false) {
}

Scene::Scene (JSONLog *log) : m_finished(false), m_log(log), m_started(false) {
	// TODO placeholders
	m_log->startScene(InputStatus(), InputStatus());
}

Scene::~Scene (void) {
	for (MeshVec::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		delete (*it);
	}

	if (m_log != nullptr) {	
		m_log->endScene();
	}
}

bool Scene::init (D3DRenderer *renderer) {
	m_renderer = renderer;
	return initMeshes();
}

const Scene::MeshVec& Scene::getMeshes (void) {
	return m_meshes;
}

Mesh* Scene::addMesh (std::string meshName, Mesh* m) {
	// Overwrite any current meshes with the same name
	if (m_meshNames.count(meshName) != 0) {
		removeMesh(meshName);
	}
	m_meshes.push_back(m);
	m_meshNames[meshName] = m;
	return m;
}
Mesh* Scene::addMesh (std::string meshName, std::string filename) {
	return addMesh(meshName, new D3DMesh(filename.c_str(), m_renderer, true));
}

void Scene::removeMesh (std::string meshName) {
	Mesh *mesh = m_meshNames[meshName];
	m_meshes.remove(mesh);
	m_meshNames.erase(meshName);
	delete mesh;
}

void Scene::processInput (InputStatus &input) {
	if (m_log != nullptr && m_started) {
		m_log->logInput(input);		
	}
	InputStatus deltaInput = input - m_lastInput;
	m_lastInput = input;

	this->_processInput(input, deltaInput);
}
