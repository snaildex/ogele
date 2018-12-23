#version 430 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormals;
layout(location = 2) in vec3 vTangents;
layout(location = 3) in vec3 vBitangents;
layout(location = 4) in vec4 vColors0;
layout(location = 5) in vec4 vColors1;
layout(location = 6) in vec4 vColors2;
layout(location = 7) in vec4 vColors3;
layout(location = 8) in vec3 vTexCoord0;
layout(location = 9) in vec3 vTexCoord1;
layout(location = 10) in vec3 vTexCoord2;
layout(location = 11) in vec3 vTexCoord3;

uniform dmat4 MVP;

out vec3 Norm;

void main(){
	dvec4 pos = MVP*dvec4(vPosition,1);
	Norm = vNormals;
	gl_Position=vec4(pos);
}