#include "CalibrationExperiment.h"
#include "CalibrationScene.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

CalibrationExperiment::CalibrationExperiment(void) : Experiment("calib.json")
{
}


CalibrationExperiment::~CalibrationExperiment(void)
{
}

Scene* CalibrationExperiment::getNextScene(void) {
	if (m_sceneIdx == m_numScenes) {
		return NULL;
	}

	delete m_curScene;
	m_curScene = new CalibrationScene(&alignment);
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