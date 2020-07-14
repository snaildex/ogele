#version 430 core

layout( points ) in;
layout( triangle_strip , max_vertices=4) out;

in vec4 Color[];
in vec4 Size[];
in float Index[];
in float Life[];

out vec4 fColor;
out vec3 screenPos;
out vec3 uv;

void main() {
	if(Life[0]>0.0001)
	{
		fColor=Color[0];
		vec2 size = vec2(Size[0].x,Size[0].y);
		vec4 pos = gl_in[0].gl_Position/gl_in[0].gl_Position.w;
		if(pos.z<=0 || pos.x<(-1-size.x) || pos.x>(1+size.x) || pos.y<(-1-size.y) || pos.y>(1+size.y)) return;
		gl_Position = pos + vec4(-size.x,-size.y,0,0);
		screenPos.xyz = gl_Position.xyz;
		uv.z = Index[0];
		uv.xy = vec2(0,1);
		EmitVertex();
		gl_Position = pos + vec4(size.x,-size.y,0,0);
		screenPos.xyz = gl_Position.xyz;
		uv.xy = vec2(1,1);
		EmitVertex();
		gl_Position = pos + vec4(-size.x,size.y,0,0);
		screenPos.xyz = gl_Position.xyz;
		uv.xy = vec2(0,0);
		EmitVertex();
		gl_Position = pos + vec4(size.x,size.y,0,0);
		screenPos.xyz = gl_Position.xyz;
		uv.xy = vec2(1,0);
		EmitVertex();
		EndPrimitive();
	}
}
