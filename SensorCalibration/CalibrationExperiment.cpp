#include "CalibrationExperiment.h"
#include "CalibrationScene.h"
#include "InteractionSpace.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

CalibrationExperiment::CalibrationExperiment(void) : Experiment("calib.json")
{
	for (double i = 0.1; i < 0.7; i += 0.23) {
		for (int j = -1; j <= 1; ++j) {
			for (int k = -1; k <= 1; ++k) {
				double x = k*InteractionSpace::maxpoint().x();
				double y = j*InteractionSpace::maxpoint().y();
				double z = i*InteractionSpace::maxpoint().z();
				positions.push_back(InteractionSpace::closestPointInVolume(Eigen::Vector3f(x, y, z)));
			}
		}
	}
	m_numScenes = 27;
}


CalibrationExperiment::~CalibrationExperiment(void)
{
}

Scene* CalibrationExperiment::getNextScene(void) {
	if (m_sceneIdx == m_numScenes) {
		return NULL;
	}

	delete m_curScene;
	m_curScene = new CalibrationScene(&alignment, positions[m_sceneIdx]);
	++m_sceneIdx;
	return m_curScene;
}

void CalibrationExperiment::writeTransform(void) {
	Eigen::MatrixXd transform = alignment.solveLeastSquares().matrix();
	string filename = "devicecalibration.calib";
	ofstream out(filename);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			out << transform(i,j) << ", ";
		}
		out << endl;
	}
}