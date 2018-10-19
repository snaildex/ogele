#version 430 core

layout( points ) in;
layout( triangle_strip , max_vertices=4) out;

uniform dmat4 VP;
uniform int Step;
uniform sampler2D Heights;
uniform sampler2D Normals;
uniform dvec3 Offset;
uniform dvec3 CamPos;
uniform dvec2 MapSize;
uniform double Time;

in ivec2 vcoord[];

out vec4 WPosDepth;
out vec3 Normal;
out vec2 UV;

const double size=0.3;
const double freq=0.1;
const float windAmplitude=0.03;

void Vertex (dvec3 wpos, vec2 uv){
	dvec4 pos=VP*dvec4(wpos,1);
	WPosDepth=vec4(wpos,pos.z/pos.w);
	UV=uv;
	gl_Position=vec4(pos);
	EmitVertex();
}

float pseudoRand(dvec2 co){
  return fract(sin(float(dot(co.xy ,dvec2(12.9898,78.233)))) * 43758.5453);
}

dvec3 ProjectOnPlane(dvec3 p, dvec3 normal, dvec3 origin)
{
			double l = dot(p - origin, normal);
			return p - normal*l;
}

#define WIND(vertex) sin(float(Time+sin(float(vertex.x)) + vertex.z) )

dvec3  Wind(dvec3 worldPos, dvec3 normal, dvec3 origin)
{
	return ProjectOnPlane(worldPos + windAmplitude * WIND(worldPos)*dvec3(1, 0, 0),normal,origin);
	//return worldPos + windAmplitude * WIND(worldPos)*dvec3(1, 0, 0);
}

void main() {
	dvec2 coord=round(CamPos.xz/freq)*freq+dvec2(vcoord[0])*freq;
	dvec2 noise=dvec2(pseudoRand(coord),pseudoRand(coord+dvec2(228,1488)))*freq;
	coord+=noise;
	vec2 texCoord=vec2((coord-Offset.xz)/MapSize)+0.5/textureSize(Heights,0);
	float height=texture(Heights,texCoord).r;
	dvec3 normal=dvec3(texture(Normals,texCoord).xyz);
	dvec3 wpos = dvec3(coord.x,height,coord.y);
	dvec3 view=normalize(CamPos-wpos);
	dvec3 side=normalize(cross(view,normal));
	//Normal=vec3(cross(vec3(0,1,0),side));
	Normal=vec3(normal);
	side/=2;
	const dvec3 up=normal;
	dvec3 top=wpos+normal*size;
	top=Wind(wpos,normal,top);
	dvec3 cpos;
	cpos=wpos+side*size;
	Vertex(cpos,vec2(1,1));
	cpos=wpos-side*size;
	Vertex(cpos,vec2(0,1));
	cpos=top+side*size;
	Vertex(cpos,vec2(1,0));
	cpos=top-side*size;
	Vertex(cpos,vec2(0,0));
	EndPrimitive();
}
