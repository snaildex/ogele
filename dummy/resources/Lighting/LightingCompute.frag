#version 330 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

uniform sampler2D AlbedoRough;
uniform sampler2D PosDepth;
uniform sampler2D NormalMetal;
uniform sampler2D Emission;
uniform sampler2D Clouds;
uniform vec3 sunDir;

layout(location = 0) out vec3 Result;

#define PI 3.14159265359
#include "includes/DataTypes.glsl"
#include "includes/PBRCore.glsl"
#include "includes/Atmosphere.glsl"

void main()
{
	vec3 view=normalize(LookDir);
    vec4 albedoRough=texture(AlbedoRough,UV);
	vec4 posDepth=texture(PosDepth,UV);
	vec4 normalMetal=texture(NormalMetal,UV);
	vec4 emission=texture(Emission,UV);
	vec3 ldir=normalize(vec3(1,1,1));
	vec3 lcol=vec3(1)*2;
    vec3 normal = normalize(normalMetal.xyz);

    sceneData scene=sceneData(
        (posDepth.w==1) ? vec3(0) : albedoRough.rgb,
        (posDepth.w==1) ? FarPos : posDepth.xyz,
        NearPos,
        normal,
        view,
        albedoRough.a,
        normalMetal.w,
        emission.rgb,
        posDepth.w,
        1.0
    );
    lightData light=lightData(
        sunDir,
        lcol
    );
	
	vec2 cloudPixelDelta=1.5*vec2(1)/textureSize(Clouds,0);
	vec4 cloud=textureLod(Clouds,UV,0);
	cloud+=textureLod(Clouds,UV+vec2(cloudPixelDelta.x,0),0);
	cloud+=textureLod(Clouds,UV-vec2(cloudPixelDelta.x,0),0);
	cloud+=textureLod(Clouds,UV+vec2(0,cloudPixelDelta.y),0);
	cloud+=textureLod(Clouds,UV-vec2(0,cloudPixelDelta.y),0);
	cloud/=5;
	
    float realDepth=scene.depth==1 ? 1e8 : distance(NearPos,scene.pos);
    vec3 color=scene.depth==1 ? vec3(0) : DirectLighting(scene,light)+AmbientLighting(scene);
	color+=emission.rgb;
    color=atmosphere(-view,NearPos,sunDir,color,realDepth,float(scene.depth==1));
    if(scene.depth==1) color=color*cloud.a+cloud.rgb;
	Result=color;
}