#include "Alignment.h"
#include "Eigen/Dense"

using namespace Eigen;
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
	int neq = 3*inputPoints.size();

	MatrixXd data;
	data.resize(neq, 12);
	data.fill(0);

	int row = 0;
	for (int i = 0; i < inputPoints.size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			data(row, j*4 + 0) = inputPoints[i].x();
			data(row, j*4 + 1) = inputPoints[i].y();
			data(row, j*4 + 2) = inputPoints[i].z();
			data(row, j*4 + 3) = 1;
			++row;
		}
	}

	VectorXd coords;
	coords.resize(neq);
	row = 0;
	for (int i = 0; i < outputPoints.size(); ++i) {
		coords(row++) = outputPoints[i].x();
		coords(row++) = outputPoints[i].y();
		coords(row++) = outputPoints[i].z();
	}

	VectorXd result = data.colPivHouseholderQr().solve(coords);
	Matrix<double, 4, 4> transform;
	transform.fill(0);
	transform(3,3) = 1;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 4; ++j) {
			transform(i,j) = result(4*i + j);
		}
	}

	return Xform(transform);
}