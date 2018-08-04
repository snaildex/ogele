#version 330 core

const vec3 Light=normalize(vec3(1,1,1));
const vec3 GrassColor=vec3(0.05,0.3,0);
const vec3 DirtColor=vec3(0.3,0.1,0);
const vec3 SandColor=vec3(0.8,0.8,0.6);

in vec3 WNorm;
in vec4 WPosDepth;

layout(location = 0) out vec4 AlbedoRough;
layout(location = 1) out vec4 PosDepth;
layout(location = 2) out vec4 NormalMetal;
layout(location = 3) out vec4 Emission;

void main()
{
vec3 norm=normalize(WNorm);
vec3 col=mix(DirtColor,GrassColor,smoothstep(0.8,0.9,norm.y));
col=mix(SandColor,col,smoothstep(1,2,WPosDepth.y));
AlbedoRough=vec4(col,0.9);
PosDepth=WPosDepth;
NormalMetal=vec4(norm,0.0);
Emission=vec4(0);
}