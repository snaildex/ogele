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
uniform samplerCube Skybox;

layout(location = 0) out vec3 Result;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 10.0;

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

float DistributionGGX(float NdotH, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH2 = NdotH*NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return a2 / max(denom, 0.001);
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}
float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}
vec3 fresnelSchlick(float HdotV, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}
vec3 fresnelSchlickRoughness(float HdotV, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - HdotV, 5.0);
}
vec3 DirectLighting(sceneData scene, lightData light)
{
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, scene.albedo, scene.metallic);
    vec3 H = normalize(scene.view + light.dir);
    float NdotV = mdot(scene.normal, scene.view);
    float NdotL = mdot(scene.normal, light.dir);
    float NdotH = mdot(scene.normal, H);
    float HdotV = mdot(H, scene.view);
    float NDF = DistributionGGX(NdotH, scene.roughness);
    float G   = GeometrySmith(NdotV, NdotL, scene.roughness);
    vec3 F    = fresnelSchlick(HdotV, F0);
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * NdotV * NdotL;
    vec3 specular = nominator / max(denominator, 0.001);
    vec3 kD = vec3(1.0) - F;
    kD *= 1.0 - scene.metallic;
    return (kD * scene.albedo / PI + specular) * light.col * NdotL;
}
vec3 AmbientLighting(sceneData scene)
{
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, scene.albedo, scene.metallic);
    float NdotV = mdot(scene.normal, scene.view);
    vec3 F = fresnelSchlickRoughness(NdotV, F0, scene.roughness);
    vec3 kD = 1.0 - F;
    kD *= 1.0 - scene.metallic;
    vec3 irradiance = textureLod(Skybox, scene.normal,MAX_REFLECTION_LOD).rgb;
    vec3 diffuse = irradiance * scene.albedo;
    vec3 reflection=reflect(scene.view,scene.normal);
    reflection.y*=-1;
    vec3 prefilteredColor = textureLod(Skybox, reflection,  scene.roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(BRDFLUT, vec2(NdotV, scene.roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    return (kD * diffuse + specular) * scene.ambientOcclusion;
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

    vec3 color=DirectLighting(scene,light);
    color+=AmbientLighting(scene);

    vec3 skyCoord=scene.view;
    skyCoord.y*=-1;
    Result=(scene.depth==1) ? texture(Skybox, skyCoord).rgb : color;
}