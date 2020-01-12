#version 330 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

uniform sampler2D AlbedoRough;
uniform sampler2D PosDepth;
uniform sampler2D NormalMetal;
uniform sampler2D Emission;
uniform vec3 sunDir;

layout(location = 0) out vec3 Result;

#define PI 3.14159265359

vec3 waterAmbient(vec3 view)
{
	const vec3 up = vec3(0.6,1.0,0.8);
	const vec3 water = vec3(0.6,0.8,1.0);
	const vec3 down = water*0.4;
	vec3 res = mix(water,up,max(view.y,0));
	res = mix(res,down,max(-view.y,0));
	return res;
}

void main()
{
	vec3 view=normalize(LookDir);
	vec4 albedoRough=texture(AlbedoRough,UV);
	vec4 posDepth=texture(PosDepth,UV);
	vec4 normalMetal=texture(NormalMetal,UV);
	vec4 emission=texture(Emission,UV);
	vec3 ldir=normalize(vec3(1,1,1));
	vec3 lcol=vec3(1)*2;
	vec3 normal = normalize(normalMetal.xyz);
	if(posDepth.a==1) {
		Result=waterAmbient(-view);
		return;
	}
	Result=albedoRough.rgb * (waterAmbient(normal) + vec3(0.5)*normal.y);
	gl_FragDepth=posDepth.w;
}