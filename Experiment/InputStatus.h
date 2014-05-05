#pragma once
#include "Eigen\Eigen"

class InputStatus
{
public:
	// Constructors
	InputStatus() 
		: pos(0,0,0), rot(1,0,0,0) { ; }
	InputStatus(double x, double y, double z) 
		: pos(x,y,z), rot(1,0,0,0) { ; }
	InputStatus(Eigen::Vector3d position) 
		: pos(position), rot(1,0,0,0) { ; }
	InputStatus(Eigen::Vector4d position) 
		: pos(position[0], position[1], position[2]), rot(1,0,0,0) { ; }

	InputStatus(InputStatus& inputstatus) : pos(inputstatus.pos), rot(inputstatus.rot), flags(inputstatus.flags) { ; }
	InputStatus(Eigen::Vector3d position, Eigen::Quaterniond rotation, int inputflags)
		: pos(position), rot(rotation), flags(inputflags) { ; }
	InputStatus(Eigen::Vector4d position, Eigen::Quaterniond rotation, int inputflags)
		: pos(position[0], position[1], position[2]), rot(rotation), flags(inputflags) { ; }

	InputStatus(double x, double y, double z, 
		        double yaw, double pitch, double roll, int inputflags)
				: pos(x,y,z), flags(inputflags) 
	{
		// Ref: http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
		double cy = cos(yaw/2);
		double cp = cos(pitch/2);
		double cr = cos(roll/2);
		double sy = sin(yaw/2);
		double sp = sin(pitch/2);
		double sr = sin(roll/2);

		double a = cr*cp*cy + sr*sp*sy;
		double b = sr*cp*cy - cr*sp*sy;
		double c = cr*sp*cy + sr*cp*sy;
		double d = cr*cp*sy - sr*sp*cy;

		rot = Eigen::Quaterniond(a,b,c,d);
	}
	
	
	// Accessors
	double operator[](int n) const { return pos[n]; }
	double x() const { return pos.x(); }
	double y() const { return pos.y(); }
	double z() const { return pos.z(); }
	Eigen::Vector4d position() const { return Eigen::Vector4d(pos[0], pos[1], pos[2], 1.); }

	Eigen::Quaterniond rotation() const { return rot; }

	enum {
		INPUTFLAG_NONE = 0,
		INPUTFLAG_SELECT = 1,
		INPUTFLAG_DESELECT = 2,
		// INPUTFLAG_XXX = 4,
	};

private:
	Eigen::Vector3d pos;
	Eigen::Quaterniond rot;
	
	int flags;
};
