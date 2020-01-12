#version 400 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;

uniform vec3 sunDir;

layout(location = 0) out vec4 Result;

#define PI 3.14159265359
#include "includes/Clouds.glsl"

void main()
{
	vec3 view=normalize(LookDir);
	Result=clouds(NearPos,-view, sunDir);
	//Result=vec4(NearPos,1);
}