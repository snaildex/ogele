#version 330 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

uniform sampler2D AlbedoRough;
uniform sampler2D PosDepth;
uniform sampler2D NormalMetal;
uniform sampler2D Emission;
uniform vec3 sunDir;

layout(location = 0) out vec3 Result;

#define PI 3.14159265359
#include "includes/DataTypes.glsl"
#include "includes/PBRCore.glsl"
#include "includes/Atmosphere.glsl"
#include "includes/Ocean.glsl"

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
        albedoRough.rgb,
        (posDepth.w==1) ? FarPos : posDepth.xyz,
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

    vec3 color=DirectLighting(scene,light)+AmbientLighting(scene);
    vec3 atmo=atmosphere(-view, NearPos, sunDir);
    if(scene.depth==1) color=atmo;

    float realDepth=distance(NearPos,scene.pos);
    vec3 waterPos;
    if(IntersectRayPlane(NearPos,-view,waterPos)){
        float waterDist=distance(NearPos,waterPos);
        float waterDepth= NearPos.y > 0 ? distance(waterPos,scene.pos) : distance(NearPos,waterPos);
        if(waterDist<realDepth || scene.depth==1){
            color=AbsorbWater(color,waterDepth);
            vec3 waterCol=vec3(0.03,0.06,0.18);
            color+=waterCol*ScatterWater(waterPos,-view*min(waterDepth,5), light);
        }
        Result=color;
        //Result=vec3(waterDepth);
    }else
    Result=(scene.depth==1) ? atmo : color;

}