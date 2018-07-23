#version 330 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

uniform sampler2D AlbedoRough;
uniform sampler2D PosDepth;
uniform sampler2D NormalMetal;
uniform sampler2D Emission;

uniform sampler2D Diffuse;
uniform sampler2D Specular;

layout(location = 0) out vec3 Color;

void main()
{
	vec3 view=normalize(LookDir);
	vec4 albedoRough=texture(AlbedoRough,UV);
	vec4 posDepth=texture(PosDepth,UV);
	vec4 normalMetal=texture(NormalMetal,UV);
	vec4 emission=texture(Emission,UV);

	vec3 normal=normalMetal.xyz;
	vec3 pos=posDepth.xyz;
	float depth=posDepth.w;
	vec3 albedo=albedoRough.rgb;
	float rough=albedoRough.a;
	float metal=normalMetal.w;

	vec3 diffuse=texture(Diffuse,UV).rgb;
	vec3 specular=texture(Specular,UV).rgb;

	Color=(diffuse+specular)*albedo;
	if(depth==1)
    		Color=vec3(0.4,0.6,0.8);
}