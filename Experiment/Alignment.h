#pragma once
#define EIGEN_DONT_ALIGN_STATICALLY 1
#include "Eigen/Eigen"
#include <vector>

typedef Eigen::Transform<double, 3, Eigen::Affine> Xform;

class Alignment
{
public:	
	Alignment(void);
	~Alignment(void);

	void addCorrespondence(Eigen::Vector3f a, Eigen::Vector3f b);

	Xform solveRansac();
	Xform solveLeastSquares();
	
private:
	std::vector<Eigen::Vector3f> inputPoints;
	std::vector<Eigen::Vector3f> outputPoints;
};

