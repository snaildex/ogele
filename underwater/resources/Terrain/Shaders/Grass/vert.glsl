#version 430 core

layout(location = 0) in ivec2 vCoord;
out ivec2 vcoord;

void main()
{
vcoord=vCoord;
gl_Position=vec4(1);
}