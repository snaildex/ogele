//
// Created by ??????? on 22.07.2018.
//

#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    trquat RotationBetweenVectors(const trvec3 &from, const trvec3 &to) {
        trscalar dt = dot(from, to);
        trscalar rotationAngle = acos(dt);
        trvec3 rotationAxis = normalize(cross(from, to));
        return normalize(angleAxis(rotationAngle, rotationAxis));
    }

    trscalar AngleBetween(const trvec3 &a, const trvec3 &b, const trvec3 &n) {
        return atan2(dot(cross(b, a), n), dot(a, b));
    }

    trquat LookAtRotation(const trvec3 &dir, const trvec3 &up) {
        trvec3 afwd(0, 0, -1);
        trquat yrot = angleAxis(AngleBetween(dir, afwd, up), up);
        trvec3 fwd = normalize(yrot * afwd);
        trvec3 rt = cross(fwd, up);
        trquat xrot = angleAxis(AngleBetween(dir, fwd, rt), rt);
        return xrot * yrot;
    }

    trvec3 ProjectOnPlane(const trvec3 &vec, const trvec3 &up) {
        return vec - proj(vec, up);
    }
}
