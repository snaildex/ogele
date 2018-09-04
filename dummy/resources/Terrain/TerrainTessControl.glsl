#version 430 core

layout(vertices = 4) out;

in float Dist[];

uniform float MaxLOD;
uniform float LODDist;

float lodFactor(float dist){
    float t=MaxLOD-dist/LODDist+0.7;
    return exp2(clamp(t,0,MaxLOD));
}

void main() {
    float centerDist=(Dist[0]+Dist[1]+Dist[2]+Dist[3])/4;
    float ct=lodFactor(centerDist);

    float t0 = lodFactor((Dist[1]+Dist[2])/2);
    float t1 = lodFactor((Dist[0]+Dist[1])/2);
    float t2 = lodFactor((Dist[3]+Dist[0])/2);
    float t3 = lodFactor((Dist[2]+Dist[3])/2);

    gl_TessLevelInner[0] = (t1+t2)/2;
    gl_TessLevelInner[1] = (t0+t3)/2;
    gl_TessLevelOuter[0] = t0;
    gl_TessLevelOuter[1] = t1;
    gl_TessLevelOuter[2] = t2;
    gl_TessLevelOuter[3] = t3;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
