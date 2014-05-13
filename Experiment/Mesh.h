#pragma once
#include "Renderer.h"
#include <exception>
#include <string>
#define EIGEN_DONT_ALIGN_STATICALLY 1
#include "Eigen\Eigen"

class OFFFileFormatError : public std::exception {
public:
	OFFFileFormatError(std::string s){
		err = s;
	}
	std::string err;
};

class Mesh
{
public:
	Mesh(void);
	Mesh(const char* filename, Renderer* r, bool reverse=false, bool flatshaded=false);
	~Mesh(void);

	virtual void draw() = 0;
	virtual void drawWireframe() = 0;
	virtual void writeBuffer() = 0;
	bool ReadOff(const char* filename, bool reverse=false);

	const Eigen::Vector3f getTranslation() { return translation; }
	const Eigen::Quaternionf getRotation() { return rotation; }
	const Eigen::Vector3f getScale() { return scale; }
	
	void setColor(float x, float y, float z, float a=1.0f) {
		for (int i = 0; i < nvert; ++i) {
			colors[4*i] = x; colors[4*i+1] = y; colors[4*i+2] = z; colors[4*i+3] = a;
		}
		writeBuffer();
	}
	const float* getColor() {
		return colors;
	}

	void getTranslation (float *t) {
		t[0] = translation[0];
		t[1] = translation[1];
		t[2] = translation[2];
	}
	void translateBy(float x, float y, float z) {
		translation[0] += x; translation[1] += y; translation[2] += z;
	}
	void translateBy(const float* x) {
		translation[0] += x[0]; translation[1] += x[1]; translation[2] += x[2];
	}
	void translateBy(const Eigen::Vector3f& x) {
		translation += x;
	}
	void setTranslation(float x, float y, float z) {
		translation[0] = x; translation[1] = y; translation[2] = z;
	}
	void setTranslation(const float* x) {
		translation[0] = x[0]; translation[1] = x[1]; translation[2] = x[2];
	}
	void setTranslation(const Eigen::Vector3f& x) {
		translation = x;
	}
	void rotateBy(float yaw, float pitch, float roll) {
		rotation = Eigen::AngleAxisf(yaw, Eigen::Vector3f(0,1,0))
			     * Eigen::AngleAxisf(pitch, Eigen::Vector3f(1,0,0))
				 * Eigen::AngleAxisf(roll, Eigen::Vector3f(0,0,1))
				 * rotation;
	}
	void rotateBy(const float* x) {
		rotateBy(x[0],x[1],x[2]);
	}
	void rotateBy(float x, float y, float z, float w) {
		Eigen::Quaternionf q(w,x,y,z);
		rotation = q.normalized()*rotation;
	}
	void rotateBy(const Eigen::Quaternionf& x) {
		rotation = x*rotation;
	}
	void setRotation(float x, float y, float z, float w) {
		rotation = Eigen::Quaternionf(w,x,y,z);
	}
	void setRotation(const Eigen::Quaternionf& x) {
		rotation = x;
	}
	void scaleBy(float x, float y, float z) {
		scale[0] *= x; scale[1] *= y; scale[2] *= z;
	}
	void scaleBy(const float* x) {
		scale[0] *= x[0]; scale[1] *= x[1]; scale[2] *= x[2];
	}
	void setScale(float x, float y, float z) {
		scale[0] = x; scale[1] = y; scale[2] = z;
	}
	void setScale(const float* x) {
		scale[0] = x[0]; scale[1] = x[1]; scale[2] = x[2];
	}
	void setScale(float x) {
		scale[0] = x; scale[1] = x; scale[2] = x;
	}
	void scaleBy(float x) {
		scale[0] *= x; scale[1] *= x; scale[2] *= x;
	}
protected:
	void calcNormals();
	void calcFaceNormals();

	Renderer* render;

	float* vertices;
	float* colors;
	float* normals;
	float* texture;
	unsigned int* indices;

	int nvert, nface, nedge;
	bool use_color;
	bool isflatshaded;

	Eigen::Vector3f translation;
	Eigen::Vector3f scale;
	Eigen::Quaternionf rotation;
};


inline float dist2(const float a[3], const float b[3]) {
	return (a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2]);
}

inline void cross(const float* a, const float* b, float* c) {
	c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = a[2]*b[0] - a[0]*b[2];
    c[2] = a[0]*b[1] - a[1]*b[0];
}

inline void normalize(float* a) {
	float tot = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
	a[0] /= tot;
	a[1] /= tot;
	a[2] /= tot;
}