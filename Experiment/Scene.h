#pragma once

#include <vector>

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
	typedef std::vector<Mesh*> MeshVec;

	Scene (void);
	~Scene (void);

	bool init (D3DRenderer *render);
    const MeshVec& getMeshes (void);
    bool finished (void);	
	void processInput (const InputStatus &input, const InputStatus &deltaInput);

private:
	MeshVec m_meshes;
	bool m_finished;
};
