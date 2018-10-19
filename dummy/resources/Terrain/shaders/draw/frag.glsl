#version 330 core

const vec3 Light=normalize(vec3(1,1,1));
const vec3 GrassColor=vec3(0.05,0.3,0);
const vec3 DirtColor=vec3(0.3,0.1,0);
const vec3 SandColor=vec3(0.8,0.8,0.6);

in vec2 WUV;
in vec4 WPosDepth;

uniform sampler2D Normals;
uniform sampler2DArray Albedo;
uniform sampler2DArray Roughness;
uniform sampler2DArray Normal;

layout(location = 0) out vec4 AlbedoRough;
layout(location = 1) out vec4 PosDepth;
layout(location = 2) out vec4 NormalMetal;
layout(location = 3) out vec4 Emission;

void main()
{
vec3 vnorm=normalize(texture(Normals,WUV).rgb);
vec3 tang=vnorm.zyx * vec3 (1.0, -1.0, 1.0);
vec3 btang=cross(vnorm,tang);
mat3 TBN = mat3(tang,btang,vnorm);

vec3 texCoord=vec3(WPosDepth.xz,0);
vec3 norm=normalize(TBN*(texture(Normal,texCoord).rgb*2-1));
vec3 col=texture(Albedo,texCoord).rgb;
float rough=1-texture(Roughness,texCoord).r;

AlbedoRough=vec4(col,rough);
PosDepth=WPosDepth;
NormalMetal=vec4(norm,0.0);
Emission=vec4(0);
}