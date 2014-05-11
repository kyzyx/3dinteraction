#include "Alignment.h"


Alignment::Alignment(void)
{
}


Alignment::~Alignment(void)
{
}

void Alignment::addCorrespondence(Eigen::Vector3f a, Eigen::Vector3f b) {
	inputPoints.push_back(a);
	outputPoints.push_back(b);
}

Xform Alignment::solveRansac() {
	// FIXME
	return Xform::Identity();
}
Xform Alignment::solveLeastSquares() {
	// FIXME
	return Xform::Identity();
}