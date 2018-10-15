#version 400 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 nearPos;
layout(location = 2) in vec3 farPos;

out vec3 NearPos;
out vec3 FarPos;
out vec3 LookDir;
out vec2 UV;
out vec2 PrevUV;

uniform dmat4 PrevVP;
uniform dmat4 IVP;

dvec4 mul(dmat4 mat, dvec4 vec){
	dvec4 res=mat*vec;
	res/=res.w;
	return res;
}

void main()
{
	gl_Position = vec4(nearPos.xy,0,1);	
	dvec4 fPos=mul(IVP, dvec4(farPos,1));
	dvec4 nPos=mul(IVP, dvec4(nearPos,1));
	LookDir=vec3(normalize(nPos.xyz-fPos.xyz));
	FarPos=vec3(fPos.xyz);
	NearPos=vec3(nPos.xyz);
    UV=uv;
	dvec4 prevfPos=mul(PrevVP, fPos);
	PrevUV=vec2(prevfPos.xy/2)+vec2(0.5);
}