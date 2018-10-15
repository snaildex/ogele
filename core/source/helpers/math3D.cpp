#include <helpers/math3D.h>
#include <glm/gtx/projection.hpp>

using namespace std;
using namespace glm;

namespace ogele {

    dquat RotationBetweenVectors(const dvec3 &from, const dvec3 &to) {
        double dt = dot(from, to);
		double rotationAngle = acos(dt);
        dvec3 rotationAxis = normalize(cross(from, to));
        return normalize(angleAxis(rotationAngle, rotationAxis));
    }

	double AngleBetween(const dvec3 &a, const dvec3 &b, const dvec3 &n) {
        return atan2(dot(cross(b, a), n), dot(a, b));
    }

    dquat LookAtRotation(const dvec3 &dir, const dvec3 &up) {
        dvec3 afwd(0, 0, -1);
        dquat yrot = angleAxis(AngleBetween(dir, afwd, up), up);
        dvec3 fwd = normalize(yrot * afwd);
        dvec3 rt = cross(fwd, up);
        dquat xrot = angleAxis(AngleBetween(dir, fwd, rt), rt);
        return xrot * yrot;
    }

    dvec3 ProjectOnPlane(const dvec3 &vec, const dvec3 &up) {
        return vec - proj(vec, up);
    }
}
