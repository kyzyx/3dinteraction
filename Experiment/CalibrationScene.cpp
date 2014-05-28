#include "CalibrationScene.h"
#include "InteractionSpace.h"


CalibrationScene::CalibrationScene(Alignment* a)
{
	alignment = a;
	pos = InteractionSpace::randomPointInVolume();
}

CalibrationScene::CalibrationScene(Alignment* a, Eigen::Vector3f p)
{
	alignment = a;
	pos = p;
}

CalibrationScene::~CalibrationScene(void)
{
}

bool CalibrationScene::initMeshes(void) {
	Mesh* mesh = addMesh("target", "ico.off");
	mesh->setColor(1,0.6,0);
	mesh->setTranslation(pos[0],pos[1],pos[2]);
	mesh->setScale(1,1,1);
	return true;
}

void CalibrationScene::_processInput(InputStatus &input, InputStatus &deltaInput) {
	if (input.isFlagSet(InputStatus::INPUTFLAG_SELECT)) {
		alignment->addCorrespondence(input.pos.cast<float>(), pos);
		m_finished = true;
	}
}
