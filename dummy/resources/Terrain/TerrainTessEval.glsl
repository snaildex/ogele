#version 430 core

layout(quads, fractional_odd_spacing, ccw) in;

out vec2 WUV;
out vec4 WPosDepth;

uniform sampler2D Heights;

uniform mat4 VP;
uniform vec3 Offset;
uniform int Step;
layout(std430) buffer Offsets
{
     ivec2 offsets[];
};

void main(){
        float u = gl_TessCoord.x;
        float v = gl_TessCoord.y;
        vec4 a = mix(gl_in[1].gl_Position, gl_in[0].gl_Position, u);
        vec4 b = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, u);
        vec4 position = mix(a, b, v);

        vec2 coord=(position.xz+vec2(0.5))/textureSize(Heights,0);
        vec3 WPos=vec3(position.x,texture(Heights,coord).r,position.z)+Offset;
        vec4 pos=VP*vec4(WPos,1);
        WPosDepth=vec4(WPos,pos.z/pos.w);
        WUV=coord;
        gl_Position = pos;
}