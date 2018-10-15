#version 330 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;
in vec2 PrevUV;

uniform vec3 sunDir;
uniform sampler2D prevFrame;

layout(location = 0) out vec4 Result;

#define PI 3.14159265359
#include "includes/Clouds.glsl"

void main()
{
	vec3 view=normalize(LookDir);
	vec4 curr=clouds(NearPos,-view, sunDir);
	vec4 prev=(distance(PrevUV,UV)<0.005)?textureLod(prevFrame,PrevUV,0):curr;
    Result=(curr+prev)/2;
}