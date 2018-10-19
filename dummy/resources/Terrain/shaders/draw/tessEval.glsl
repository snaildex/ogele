#version 430 core

layout(quads, fractional_even_spacing, ccw) in;
//layout(quads, equal_spacing, ccw) in;

out vec2 WUV;
out vec4 WPosDepth;

uniform sampler2D Heights;

uniform dmat4 VP;
uniform dvec3 Offset;
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
        dvec3 WPos=dvec3(position.x,texture(Heights,coord).r,position.z)+Offset;
        dvec4 pos=VP*dvec4(WPos,1);
        WPosDepth=vec4(WPos,pos.z/pos.w);
        WUV=coord;
        gl_Position = vec4(pos);
}