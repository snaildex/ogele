#version 330 core

in vec2 UV;

uniform sampler2D HDRFrame;

layout(location = 0) out vec3 Result;

void main() {
     vec3 color=texture(HDRFrame, UV).rgb;
     color = color / (color + vec3(1.0));
     color = pow(color, vec3(1.0/2.2));
     Result=color;
}
