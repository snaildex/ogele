#version 430 core

layout(location = 0) in ivec3 vCoord;

uniform dmat4 VP;
uniform dmat4 V;
uniform dmat4 P;
uniform double Time;
uniform dvec3 Center;
uniform dvec3 CamPos;
uniform double Interval;
uniform double MinSize;
uniform double MaxSize;
uniform double Range;
uniform vec4 Color;

out vec4 vColor;
out vec4 Size;
out float Index;

double rand(dvec4 co){
  return double(fract(sin(float(dot(co ,dvec4(12.9898,78.233,1134.971,-99997.65)))) * 43758.5453));
}

void main()
{
	dvec3 pos = Center + vCoord * Interval;
	dvec3 offset = dvec3(rand(dvec4(pos,0)),rand(dvec4(pos,100)),rand(dvec4(pos,200)));
	pos+=(offset-0.5)*Interval;
	double size = mix(MinSize, MaxSize, rand(dvec4(pos,1400)));
	gl_Position=vec4(VP*dvec4(pos,1));
	dvec4 vpos=V*dvec4(pos,1);
	dvec4 s = P*dvec4(size,size,vpos.z,1);
	s/=s.w;
	Size=vec4(s);
	Index = 0;
	double fade = 1-(distance(CamPos, pos))/Range;
	fade = 0.8+fade*0.2;
	vColor=Color;
	vColor.a*=float(fade);
}