#version 430 core

layout(vertices = 4) out;

in float Factor[];

void main() {
    float t0 = (Factor[1]+Factor[2])/2;
    float t1 = (Factor[0]+Factor[1])/2;
    float t2 = (Factor[3]+Factor[0])/2;
    float t3 = (Factor[2]+Factor[3])/2;
	
    gl_TessLevelInner[0] = (t1+t2)/2;
    gl_TessLevelInner[1] = (t0+t3)/2;
    gl_TessLevelOuter[0] = t0;
    gl_TessLevelOuter[1] = t1;
    gl_TessLevelOuter[2] = t2;
    gl_TessLevelOuter[3] = t3;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
