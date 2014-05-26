#include "LeapInterface.h"
#include "timestamp.h"
#include <vector>

using namespace std;
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
	vector<int> extended;
	for (int i = 0; i < f.pointables().count(); ++i) {
		if (f.pointables()[i].isExtended()) {
			int id = f.pointables()[i].id();
			// Ignore thumb
			if (f.pointable(id).isFinger() && f.finger(id).type() != Finger::TYPE_THUMB) {
				extended.push_back(id);
			}
		}
	}
	if (extended.size() == 0) return;
	if (tracked == -1) {
		if (extended.size() > 1) return;
		tracked = extended.front();
	}
	Pointable p = f.pointable(tracked);
	if (!p.isValid() || extended.size() > 2) {
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
