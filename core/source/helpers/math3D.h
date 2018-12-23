#pragma once
namespace ogele {

    glm::dquat RotationBetweenVectors(const  glm::dvec3 &from, const  glm::dvec3 &to);

    double AngleBetween(const  glm::dvec3 &a, const  glm::dvec3 &b, const  glm::dvec3 &n);

	glm::dquat LookAtRotation(const  glm::dvec3 &dir, const  glm::dvec3 &up);

	glm::dvec3 ProjectOnPlane(const  glm::dvec3 &vec, const  glm::dvec3 &up);

}
