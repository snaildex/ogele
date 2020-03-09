#version 330 core

in vec4 WPosDepth;
in vec3 Normal;
in vec2 UV;

layout(location = 0) out vec4 AlbedoRough;
layout(location = 1) out vec4 PosDepth;
layout(location = 2) out vec4 NormalMetal;
layout(location = 3) out vec4 Emission;

uniform sampler2DArray Grass;

void main()
{
vec4 g=texture(Grass,vec3(UV,0));
if(g.a <= 0.5) 
        discard;
AlbedoRough=vec4(g.rgb,0.9);
PosDepth=WPosDepth;
NormalMetal=vec4(Normal,0.0);
Emission=vec4(0);
}