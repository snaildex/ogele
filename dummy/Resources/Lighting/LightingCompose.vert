#version 330 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 nearPos;
layout(location = 2) in vec3 farPos;

out vec3 NearPos;
out vec3 FarPos;
out vec3 LookDir;
out vec2 UV;

uniform mat4 IVP;

void main()
{
	gl_Position = vec4(nearPos.xy,0,1);	
	vec4 farPos=IVP * vec4(farPos,1);
	farPos.xyz/=farPos.w;
	vec4 nearPos=IVP * vec4(nearPos,1);
	nearPos.xyz/=nearPos.w;
	LookDir=normalize(farPos.xyz-nearPos.xyz);
	NearPos=nearPos.xyz;
	FarPos=farPos.xyz;
	UV=uv;
}