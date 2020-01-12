#version 430 core

in vec3 VNorm;
in vec4 VPosDepth;

layout(location = 0) out vec4 AlbedoRough;
layout(location = 1) out vec4 PosDepth;
layout(location = 2) out vec4 NormalMetal;
layout(location = 3) out vec4 Emission;

void main(){
	AlbedoRough = vec4(0.5,0.5,0.5,0.5);
	PosDepth = VPosDepth;
	NormalMetal = vec4(VNorm, 0.5);
	Emission=vec4(0,0,0,0);
}