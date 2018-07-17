#version 430 core

layout(location = 0) in ivec2 vCoord;
layout(location = 1) in vec2 vUV;
layout (r32f) uniform image2D Heights;
layout(rgba32f) uniform image2D Normals;

uniform mat4 VP;
uniform vec3 Offset;
layout(std430) buffer Offsets
{
     ivec2 offsets[];
};

out vec3 WNorm;
out vec4 WPosDepth;

void main()
{
ivec2 coord=vCoord+offsets[gl_InstanceID];
vec3 WPos=vec3(coord.x,imageLoad(Heights,coord).r,coord.y)+Offset;
vec4 pos=VP*vec4(WPos,1);
WPosDepth=vec4(WPos,pos.z/pos.w);
WNorm=imageLoad(Normals,coord).rgb;
gl_Position = pos;
}