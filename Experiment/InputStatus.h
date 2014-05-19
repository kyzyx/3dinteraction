#pragma once
#define EIGEN_DONT_ALIGN_STATICALLY 1
#include "Eigen\Eigen"
#include <cstdint>

class InputStatus
{
public:
	enum InputType : uint32_t {MOUSEKBD, HYDRA, LEAP, ARTAG, NONE, UNKNOWN};
	enum InputFlag : uint32_t {
		INPUTFLAG_NONE = 0,
		INPUTFLAG_SELECT = 1,
		INPUTFLAG_DESELECT = 2
		// INPUTFLAG_XXX = 4,
	};

	// Constructors
	InputStatus() 
		: pos(0,0,0), rot(1,0,0,0), inputType(UNKNOWN), timestamp(0.0) { ; }
	InputStatus(double x, double y, double z) 
		: pos(x,y,z), rot(1,0,0,0), inputType(UNKNOWN), timestamp(0.0)  { ; }
	InputStatus(Eigen::Vector3d position) 
		: pos(position), rot(1,0,0,0), inputType(UNKNOWN), timestamp(0.0)  { ; }
	InputStatus(Eigen::Vector4d position) 
		: pos(position[0], position[1], position[2]), rot(1,0,0,0), inputType(UNKNOWN), timestamp(0.0)  { ; }

	InputStatus(const InputStatus& inputstatus) : pos(inputstatus.pos), rot(inputstatus.rot), flags(inputstatus.flags), inputType(inputstatus.inputType), timestamp(inputstatus.timestamp) { ; }
	InputStatus(Eigen::Vector3d position, Eigen::Quaterniond rotation, int inputflags)
		: pos(position), rot(rotation), flags(inputflags), inputType(UNKNOWN), timestamp(0.0)  { ; }
	InputStatus(Eigen::Vector4d position, Eigen::Quaterniond rotation, int inputflags)
		: pos(position[0], position[1], position[2]), rot(rotation), flags(inputflags), inputType(UNKNOWN), timestamp(0.0)  { ; }

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
	
	
	InputStatus operator- (const InputStatus &other) const {
		InputStatus is;
		is.inputType = inputType; // save type of left operatand
		is.pos = pos - other.pos;
		is.rot = rot * other.rot.inverse();
		is.flags = flags ^ other.flags; // which flags changed
		is.timestamp = timestamp - other.timestamp;
		return is;
	}

	// Accessors
	double& operator[](int n) { return pos[n]; }
	double& x() { return pos[0]; }
	double& y() { return pos[1]; }
	double& z() { return pos[2]; }
	Eigen::Vector4d position() const { return Eigen::Vector4d(pos[0], pos[1], pos[2], 1.); }

	Eigen::Quaterniond rotation() const { return rot; }

	bool flagSet (InputFlag flag) { return (flags & (uint32_t)flag) != 0; }
	void setFlag (InputFlag flag) { flags |= (uint32_t)flag; }
	void clearFlag (InputFlag flag) { flags &= ~(uint32_t)flag; }

	Eigen::Vector3d pos;
	Eigen::Quaterniond rot;
	
	InputType inputType; // what kind of device this data came from
	double timestamp;    // when was this input event generated?
	
	uint32_t flags;
	
};
