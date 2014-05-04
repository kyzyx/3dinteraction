#pragma once

#include <vector>

#include "Mesh.h"
#include "InputInterface.h"
#include "D3DRenderer.h"

//
// SCENE
//
// A scene controls meshes on the screen and handles input
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
