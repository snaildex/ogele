//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_MATH3D_H
#define OGELE_MATH3D_H
namespace ogele {

    trquat RotationBetweenVectors(const trvec3 &from, const trvec3 &to);

    trscalar AngleBetween(const trvec3 &a, const trvec3 &b, const trvec3 &n);

    trquat LookAtRotation(const trvec3 &dir, const trvec3 &up);

    trvec3 ProjectOnPlane(const trvec3 &vec, const trvec3 &up);

}
#endif //OGELE_MATH3D_H
