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

layout(location = 0) out vec3 Diffuse;
layout(location = 1) out vec3 Specular;

const float PI = 3.14159265359;

float mdot(vec3 a, vec3 b){
	return max(dot(a,b),0);
}

float DistributionGGX(vec3 N, vec3 H, float a){
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = 3.14 * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k){
    return NdotV / (NdotV * (1.0 - k) + k);
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k){
    float ggx1 = GeometrySchlickGGX(mdot(N,V), k);
    float ggx2 = GeometrySchlickGGX(mdot(N,L), k);
	//return mdot(N,V)*mdot(N,L);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
} 

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void DirectionalLighting(vec3 albedo, float metallic, float roughness, vec3 norm, vec3 view, vec3 ldir, vec3 color)
{
	vec3 halfdir=normalize(view+ldir);	
	vec3 F0 = vec3(0.04); 
	F0      = mix(F0, albedo, metallic);	
    
	float NDF = DistributionGGX(norm, halfdir, roughness);        
    float G   = GeometrySmith(norm, view, ldir, roughness);
	vec3 F    = fresnelSchlick(mdot(halfdir, view), F0);
	
    vec3 kD = (vec3(1.0) - F)*(1.0 - metallic);   
	
	vec3 nominator    = NDF * G * F;
    float denominator = 4 * mdot(norm, view)*mdot(norm,ldir); 
	float NdotL = mdot(norm, ldir);                
    
	Specular += color * nominator / max(denominator, 0.001) * NdotL;
	Diffuse += color * kD * albedo * NdotL;
	//Diffuse +=color*G;
	//Specular += view;
	//Specular += color * fresnelSchlick(mdot(halfdir, view), F0);
}

void AmbientLighting(vec3 albedo, float ao, vec3 normal, vec3 view, vec3 reflection, float roughness, float metallic)
{	
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);
	float theta=mdot(normal, view);
	vec3 F = fresnelSchlickRoughness(theta, F0, roughness);
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;
	//vec3 irradiance = textureLod(skybox, normal,8).rgb;
	vec3 irradiance = vec3(0.4,0.6,0.8);
	vec3 diff    = irradiance * albedo;
	
	//vec3 prefilteredColor = textureLod(skybox, reflection,  roughness * 11.0).rgb;   
	//vec3 prefilteredColor = textureLod(skybox, reflection,  log2(roughness * 2048)).rgb;   
	vec3 prefilteredColor = vec3(0.4,0.6,0.8);   
	vec2 envBRDF  = texture(BRDFLUT, clamp(vec2(theta, roughness),vec2(0),vec2(1))).rg;
	
	Specular += prefilteredColor * (F * envBRDF.x + envBRDF.y);
	Diffuse += kD * diff * ao;
}

void main()
{
	Diffuse=vec3(0);
	Specular=vec3(0);
	
	vec3 view=normalize(LookDir);
	vec4 albedoRough=texture(AlbedoRough,UV);
	vec4 posDepth=texture(PosDepth,UV);
	vec4 normalMetal=texture(NormalMetal,UV);
	vec4 emission=texture(Emission,UV);

	vec3 normal=normalize(normalMetal.xyz);
	vec3 reflection = reflect(-view,normal);
	vec3 pos=posDepth.xyz;
	float depth=posDepth.w;
	vec3 albedo=albedoRough.rgb;
	float rough=albedoRough.a;
	float metal=normalMetal.w;

	vec3 ldir=normalize(vec3(1,1,1));
	vec3 lcol=vec3(1);

	AmbientLighting(albedo,1,normal,view,reflection,rough,metal);
	DirectionalLighting(albedo,metal,rough,normal,view,ldir,lcol);
}