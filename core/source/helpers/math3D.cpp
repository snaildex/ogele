#include <stdafx.h>
#include <ogele.h>

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
		dvec3 aup(0, 1, 0);
		dquat rot = angleAxis(AngleBetween(dir, afwd, aup), aup);
		dvec3 fwd = normalize(rot * afwd);
		dvec3 rt = cross(fwd, aup);
		rot = angleAxis(AngleBetween(dir, fwd, rt), rt) * rot;
		dvec3 cup = normalize(rot * aup);
		dvec3 cfwd = normalize(rot * afwd);
		rot = angleAxis(AngleBetween(up, cup, cfwd), cfwd) * rot;
		return rot;
	}

	dvec3 ProjectOnPlane(const dvec3 &vec, const dvec3 &up) {
		return vec - proj(vec, up);
	}

	dvec3 Permute(dvec3 x) { return mod(((x*34.0) + 1.0)*x, 289.0); }

	double SimplexNoise(dvec2 v) {
		const dvec4 C = dvec4(0.211324865405187, 0.366025403784439,
			-0.577350269189626, 0.024390243902439);
		dvec2 i = floor(v + dot(v, C.yy()));
		dvec2 x0 = v - i + dot(i, C.xx());
		dvec2 i1;
		i1 = (x0.x > x0.y) ? dvec2(1.0, 0.0) : dvec2(0.0, 1.0);
		dvec4 x12 = x0.xyxy + C.xxzz;
		x12.xy -= i1;
		i = mod(i, 289.0);
		dvec3 p = Permute(Permute(i.y + dvec3(0.0, i1.y, 1.0))
			+ i.x + dvec3(0.0, i1.x, 1.0));
		dvec3 m = max(0.5 - dvec3(dot(x0, x0), dot(x12.xy(), x12.xy()),
			dot(x12.zw(), x12.zw())), 0.0);
		m = m * m;
		m = m * m;
		dvec3 x = 2.0 * fract(p * C.www) - 1.0;
		dvec3 h = abs(x) - 0.5;
		dvec3 ox = floor(x + 0.5);
		dvec3 a0 = x - ox;
		m *= 1.79284291400159 - 0.85373472095314 * (a0*a0 + h * h);
		dvec3 g;
		g.x = a0.x  * x0.x + h.x  * x0.y;
		g.yz = a0.yz * x12.xz + h.yz * x12.yw;
		return 130.0 * dot(m, g);
	}

}
