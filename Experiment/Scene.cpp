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

bool Scene::init (D3DRenderer *render) {
	D3DMesh *mesh = new D3DMesh("ico.off", render);
	mesh->setColor(1,0,0);
	mesh->setTranslation(0,0,5);
	mesh->setScale(0.2f, 0.2f, 0.2f);
	m_meshes.push_back(mesh);

	mesh = new D3DMesh("spaceship.off", render, true);
	mesh->setColor(1,0,0);
	mesh->setTranslation(2,-1,7);
	mesh->setScale(0.2f, 0.2f, 0.2f);
	m_meshes.push_back(mesh);

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
	m_meshes[1]->translateBy((float)x, (float)y, (float)z);
	float t[3];
	m_meshes[1]->getTranslation(t);
	if (t[0] < -3) t[0] = -3;
	if (t[0] >  3) t[0] =  3;
	if (t[1] < -2) t[1] = -2;
	if (t[1] >  2) t[1] =  2;
	if (t[2] <  3) t[2] =  3;
	if (t[2] >  7) t[2] =  7;
    m_meshes[1]->setTranslation(t);

	Eigen::Quaterniond q = input.rot;
	double roll = atan2(2.0*q.y()*q.w() - 2.0*q.x()*q.z(), 1 - 2.0*q.y()*q.y() - 2.0*q.z()*q.z());
	double pitch = atan2(2.0*q.x()*q.w() - 2.0*q.y()*q.z(), 1 - 2.0*q.x()*q.x() - 2.0*q.z()*q.z());
	double yaw = asin(2.0*q.x()*q.y() + 2.0*q.z()*q.w());

	m_meshes[1]->setRotation(roll, pitch, yaw);

	return;
}
