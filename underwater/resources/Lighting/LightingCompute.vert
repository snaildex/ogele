#version 400 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 nearPos;
layout(location = 2) in vec3 farPos;

out vec3 NearPos;
out vec3 FarPos;
out vec3 LookDir;
out vec2 UV;

uniform dmat4 IVP;
void main()
{
	gl_Position = vec4(nearPos.xy,0,1);
	dvec4 fPos=IVP * dvec4(farPos,1);
	fPos.xyz/=fPos.w;
	dvec4 nPos=IVP * dvec4(nearPos,1);
	nPos.xyz/=nPos.w;
	LookDir=vec3(normalize(nPos.xyz-fPos.xyz));
	FarPos=vec3(fPos.xyz);
	NearPos=vec3(nPos.xyz);
    UV=uv;
}