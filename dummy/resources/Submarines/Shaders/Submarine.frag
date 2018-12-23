#version 430 core

in vec3 Norm;

const vec3 ldir=normalize(vec3(1,1,1));

layout(location = 0) out vec3 Color;

void main(){
	float l=max(dot(Norm,ldir),0);
	Color=vec3(l);
}