#include "LeapInterface.h"
#include "timestamp.h"

using namespace Leap;
LeapInterface::LeapInterface(void)
{
	tracked = -1;
	status.inputType = InputStatus::LEAP;
	baserotation = Eigen::Quaterniond::Identity();
}


LeapInterface::~LeapInterface(void)
{
}

void LeapInterface::update() {
	Frame f = controller.frame();
	int numpointables = f.pointables().count();
	if (tracked == -1) {
		if (numpointables > 1) return;
		tracked = f.pointables().frontmost().id();
	}
	Pointable p = f.pointable(tracked);
	if (!p.isValid()) {
		baserotation = status.rot;
		tracked = -1;
		return;
	}
	if (numpointables > 2) {
		baserotation = status.rot;
		tracked = -1;
		return;
	}

	status.pos[0] = p.tipPosition().x/10; // mm to cm
	status.pos[1] = p.tipPosition().y/10;
	status.pos[2] = p.tipPosition().z/10;

	Eigen::Vector3d x(0,0,-1);
	Eigen::Vector3d y(p.direction().x, p.direction().y, p.direction().z);
	Eigen::AngleAxisd t(acos(x.dot(y)), x.cross(y));
	status.rot = t*baserotation;
	status.timestamp = timestamp();
	status.flags = 0;
}
