#version 330 core

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 nearPos;
layout(location = 2) in vec3 farPos;

out vec2 UV;

void main()
{
	gl_Position = vec4(nearPos.xy,0,1);
	UV=uv;
}