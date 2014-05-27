#include "InteractionSpace.h"
#include <ctime>
#include "ARInterface.h"

using namespace Eigen;

// Bounding box shortcut
// Screen dimensions 59.790cm width 33.632cm height
Vector3f mincoords(-59.79/2+5,-33.632/2+5,0);
Vector3f maxcoords(59.79/2-5,33.632/2-5,60);

double InteractionSpace::screenheight(void) {
	return maxcoords.y()*2;
}
double InteractionSpace::screenwidth(void) {
	return maxcoords.x()*2;
}
Vector3f InteractionSpace::randomPointInVolume(void)
{
	Vector3f p;
	do {
		p = Vector3f::Random();
		p[0] *= maxcoords[0];
		p[1] *= maxcoords[1];
		p[2] = maxcoords[2]*(p[2] + 1)/2;
	} while(!inVolume(p));
	return p;
}

Vector3f normal(Vector3f a, Vector3f b, Vector3f c) {
	Vector3f x = a-b;
	Vector3f y = c-b;
	Vector3f n = x.cross(y);
	n.normalize();
	return n;
}

double checkPlane(Vector3f p, Vector3f a, Vector3f b, Vector3f c) {
	Vector3f n = normal(a,b,c);
	double d = -n.dot(b);
	return n.dot(p) + d;
}

Vector3f InteractionSpace::closestPointInVolume(Vector3f p, bool headtrack)
{
	Vector3f h = Vector3f(0,0,80);
	headtrack = headtrack && ARInterface::getInstance().isValid();
	if (headtrack) {
		Vector3f headpos = ARInterface::getInstance().getStatus().pos.cast<float>();
		if (headpos.z() > 5) h = headpos;
	}

	if (p.z() < 0) p.z() = 0;
	double ratio = p.z()/h.z();
	if (ratio > 0.75) {
		p.z() = h.z()*0.75;
		ratio = 0.75;
	}
	// Clip left
	double clip = ratio*h.x() + (1-ratio)*mincoords.x();
	if (p.x() < clip) p.x() = clip;
	// Clip right
	clip = ratio*h.x() + (1-ratio)*maxcoords.x();
	if (p.x() > clip) p.x() = clip;
	// Clip top
	clip = ratio*h.y() + (1-ratio)*mincoords.y();
	if (p.y() < clip) p.y() = clip;
	// Clip bottom
	clip = ratio*h.y() + (1-ratio)*maxcoords.y();
	if (p.y() > clip) p.y() = clip;
	return p;
}

bool InteractionSpace::inVolume(Vector3f p) {
	Vector3f tl = Vector3f(mincoords.x(), maxcoords.y(), mincoords.z());
	Vector3f tr = Vector3f(maxcoords.x(), maxcoords.y(), mincoords.z());
	Vector3f bl = Vector3f(mincoords.x(), mincoords.y(), mincoords.z());
	Vector3f br = Vector3f(maxcoords.x(), mincoords.y(), mincoords.z());
	Vector3f h = ARInterface::getInstance().getStatus().pos.cast<float>();
	if (h.z() < 5) h = Vector3f(0,0,80);
	if (p.z() < 0) return false;
	if (p.z() > h.z()*3/4) return false;
	if (checkPlane(p, tl, tr, h) > 0 ||
		checkPlane(p, tr, br, h) > 0 ||
		checkPlane(p, br, bl, h) > 0 ||
		checkPlane(p, bl, tl, h) > 0) {
		return false;
	}
	return true;
}