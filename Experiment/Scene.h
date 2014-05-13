#pragma once

#include <list>
#include <map>

#include "Mesh.h"
#include "InputInterface.h"
#include "D3DRenderer.h"
#include "JSONLog.h"

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

	// No log constructor, assumes we won't be recording input data (see CalibrationScene)
	Scene (void);

	// Log constructor, uses log to record input data (see TestScene)
	Scene (JSONLog *log);

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
	JSONLog *m_log; // memory not managed by this class

	// True if user has clicked the start position, but not the end.
	// Input logging is only done when this is true
	bool m_started; 

private:
	InputStatus m_lastInput;
};
