#version 330 core

in vec4 fColor;
in vec3 screenPos;
in vec3 uv;

uniform sampler2D PosDepth;
uniform sampler2DArray Sprites;

layout(location = 0) out vec4 Result;

void main()
{
float sd = texture(PosDepth,screenPos.xy*0.5+0.5).w;
float d = screenPos.z;
float f = clamp((sd-d) / 0.004,0,1);
Result = fColor * texture(Sprites,uv);
Result.a *= f;
}