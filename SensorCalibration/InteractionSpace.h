#pragma once
#define EIGEN_DONT_ALIGN_STATICALLY 1
#include "Eigen/Eigen"

class InteractionSpace
{
public:
	static Eigen::Vector3f randomPointInVolume();
	static Eigen::Vector3f closestPointInVolume(Eigen::Vector3f p, bool headtrack=true);
	static bool inVolume(Eigen::Vector3f p);
	static double screenwidth();
	static double screenheight();
	static Eigen::Vector3f maxpoint();
	static Eigen::Vector3f minpoint();
private:
	InteractionSpace() {;}
};

