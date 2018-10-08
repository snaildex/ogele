#version 430 core

layout(location = 0) in ivec2 vCoord;
layout(location = 1) in vec2 vUV;


uniform int Step;
uniform sampler2D Heights;
uniform vec3 Offset;
uniform float MaxLOD;
uniform float LODDist;

layout(std430) buffer Offsets
{
     ivec2 offsets[];
};

uniform vec3 CamPos;
out float Factor;

float lodFactor(float dist){
    float t=MaxLOD-dist/LODDist+0.7;
    return exp2(clamp(t,0,MaxLOD));
}

void main()
{
ivec2 coord=vCoord*Step+offsets[gl_InstanceID];
vec2 texCoord=vec2(coord+vec2(0.5))/textureSize(Heights,0);
float height=texture(Heights,texCoord).r;
gl_Position = vec4(coord.x,height,coord.y,1);
Factor=lodFactor(distance(CamPos,gl_Position.xyz+Offset));
}