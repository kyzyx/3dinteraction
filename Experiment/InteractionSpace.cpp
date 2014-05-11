#include "InteractionSpace.h"
#include <ctime>

using namespace Eigen;

// Bounding box shortcut
Vector3f mincoords(-3,-2,0);
Vector3f maxcoords(3,2,6);

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

Vector3f InteractionSpace::closestPointInVolume(Vector3f p)
{
	// FIXME: Bbox check shortcut
	for (int i = 0; i < 3; ++i) {
		if (p[i] < mincoords[i]) p[i] = mincoords[i];
		else if (p[i] > maxcoords[i]) p[i] = maxcoords[i];
	}
	return p;
}

bool InteractionSpace::inVolume(Vector3f p) {
	// Bbox check
	for (int i = 0; i < 3; ++i) {
		if (p[i] < mincoords[i] || p[i] > maxcoords[i]) return false;
	}
	// TODO: Frustum check
	return true;
}