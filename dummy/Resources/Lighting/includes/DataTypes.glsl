
struct sceneData{
    vec3 albedo;
    vec3 pos;
    vec3 eyePos;
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