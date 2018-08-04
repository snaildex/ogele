#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout(rgba32f) uniform imageCube Skybox;

uniform vec3 sunDir;
uniform vec3 camPos;

#define PI 3.14159265359
#include "includes/Atmosphere.glsl"

ivec3 facesX[6] = ivec3[6](
    ivec3(2,1,-1),
    ivec3(2,-1,1),
    ivec3(0,-1,1),
    ivec3(0,-1,1),
    ivec3(0,-1,1),
    ivec3(0,1,-1)
);
ivec3 facesY[6] = ivec3[6](
    ivec3(1,1,-1),
    ivec3(1,1,-1),
    ivec3(2,-1,1),
    ivec3(2,1,-1),
    ivec3(1,1,-1),
    ivec3(1,1,-1)
);
ivec2 depthes[6] = ivec2[6](
    ivec2(0,1),
    ivec2(0,-1),
    ivec2(1,1),
    ivec2(1,-1),
    ivec2(2,1),
    ivec2(2,-1)
);

void main()
{
    ivec3 faceX=facesX[int(gl_GlobalInvocationID.z)];
    ivec3 faceY=facesY[int(gl_GlobalInvocationID.z)];
    ivec2 depth=depthes[int(gl_GlobalInvocationID.z)];
    vec2 uv=vec2(gl_GlobalInvocationID.xy)/vec2(imageSize(Skybox));
    vec3 coord;
    coord[faceX.x]=faceX.y+faceX.z*2*uv.x;
    coord[faceY.x]=faceY.y+faceY.z*2*uv.y;
    coord[depth.x]=depth.y;
    coord=normalize(coord);
    vec3 color = atmosphere(coord,vec3(0,0,0),sunDir);
    //color = 1.0 - exp(-1.0 * color);
    imageStore(Skybox, ivec3(gl_GlobalInvocationID.xyz), vec4(color,1));
}