#version 330 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 nearPos;
layout(location = 2) in vec3 farPos;

out vec3 NearPos;
out vec3 FarPos;
out vec3 LookDir;
out vec2 UV;
out vec2 PrevUV;

uniform mat4 PrevVP;
uniform mat4 IVP;

vec4 mul(mat4 mat, vec4 vec){
	vec4 res=mat*vec;
	res/=res.w;
	return res;
}

void main()
{
	gl_Position = vec4(nearPos.xy,0,1);	
	vec4 fPos=mul(IVP, vec4(farPos,1));
	vec4 nPos=mul(IVP, vec4(nearPos,1));
	LookDir=normalize(nPos.xyz-fPos.xyz);
	FarPos=fPos.xyz;
	NearPos=nPos.xyz;
    UV=uv;
	vec4 prevfPos=mul(PrevVP, fPos);
	PrevUV=prevfPos.xy/2+vec2(0.5);
}