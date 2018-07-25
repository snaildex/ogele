#version 330 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

uniform sampler2D AlbedoRough;
uniform sampler2D PosDepth;
uniform sampler2D NormalMetal;
uniform sampler2D Emission;
uniform sampler2D BRDFLUT;

layout(location = 0) out vec3 Result;
const float PI = 3.14159265359;

float mdot(vec3 a, vec3 b)
{
    return max(dot(a,b),0.0);
}

struct sceneData{
    vec3 albedo;
    vec3 pos;
    vec3 normal;
    vec3 view;
    float roughness;
    float metallic;
    vec3 emission;
    float depth;
    float ambientOcclusion;
};
struct lightData{
      vec3 dir;
      vec3 col;
};

vec3 AmbientTerm(sceneData scene, lightData light){
    float NdotV=mdot(scene.normal,scene.view);
    float k=pow(1-NdotV,5)*0.7+0.3;
    return scene.albedo*k*vec3(0.4,0.6,0.8);
}
vec3 DiffuseTerm(sceneData scene, lightData light){
    float NdotL=mdot(scene.normal,light.dir);
    return scene.albedo*NdotL;
}
vec3 SpecularTerm(sceneData scene, lightData light){
    vec3 H=normalize(scene.view+light.dir);
    float NdotH=mdot(scene.normal,H);
    return vec3(pow(NdotH,32));
}

void main()
{
	vec3 view=normalize(LookDir);
    vec4 albedoRough=texture(AlbedoRough,UV);
	vec4 posDepth=texture(PosDepth,UV);
	vec4 normalMetal=texture(NormalMetal,UV);
	vec4 emission=texture(Emission,UV);
	vec3 ldir=normalize(vec3(1,1,1));
	vec3 lcol=vec3(1);
    vec3 normal = normalize(normalMetal.xyz);

    sceneData scene=sceneData(
        albedoRough.rgb,
        posDepth.xyz,
        normal,
        view,
        albedoRough.a,
        normalMetal.w,
        emission.rgb,
        posDepth.w,
        1.0
    );
    lightData light=lightData(
        ldir,
        lcol
    );

    vec3 color=vec3(0);
    color+=AmbientTerm(scene,light);
        color+=DiffuseTerm(scene,light);
            color+=SpecularTerm(scene,light);
    //color=vec3(mdot(scene.normal,scene.view));
    //color=scene.normal;
    Result=(scene.depth==1) ? vec3(0.4,0.6,0.8) : color;
}