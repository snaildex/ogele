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
	vec4 fPos=IVP * vec4(farPos,1);
	fPos.xyz/=fPos.w;
	vec4 nPos=IVP * vec4(nearPos,1);
	nPos.xyz/=nPos.w;
	LookDir=normalize(fPos.xyz-nPos.xyz);
	NearPos=nearPos.xyz;
	FarPos=farPos.xyz;
	UV=uv;
}