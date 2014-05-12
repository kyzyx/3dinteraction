#pragma once

#include <list>
#include <map>

#include "Mesh.h"
#include "InputInterface.h"
#include "D3DRenderer.h"

//
// SCENE
//
// A scene controls meshes on the screen and reacts to input.  A Scene in 
// our project is supposed to handle a single run of an experiment
//
class Scene {
public:
	typedef std::list<Mesh*> MeshVec;
	typedef std::map<std::string, Mesh*> MeshMap;

	Scene (void);
	~Scene (void);

	bool init (D3DRenderer *renderer);
    const MeshVec& getMeshes (void);
	bool finished (void) { return m_finished; }

	void processInput (InputStatus &input);

protected:
	virtual bool initMeshes(void) { return true; }
	virtual void _processInput (InputStatus &input, InputStatus &deltaInput){;}

	Mesh* addMesh (std::string meshName, std::string filename);
	void removeMesh (std::string meshName);

	MeshVec m_meshes;
	MeshMap m_meshNames;
	bool m_finished;
	D3DRenderer *m_renderer; // memory not managed by this class

private:
	InputStatus m_lastInput;
};
