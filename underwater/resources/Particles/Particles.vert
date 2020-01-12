#version 430 core

struct Particle {
	dvec4 PositionLife;
	dvec4 SpeedSize;
	dvec4 ForceRotation;
	dvec4 Props; //FadeStart, SizeFade, AlphaFade, TexIndex
	dvec4 Color;
};

layout(std430, binding = 0) buffer ParticleData
{
    Particle Particles[];
};

uniform dmat4 VP;
uniform dmat4 V;
uniform dmat4 P;
uniform double TimeDelta;

out vec4 Color;
out vec4 Size;
out float Index;
out float Life;

void main()
{
	Particle p = Particles[gl_VertexID];

	Color=vec4(p.Color);
	Color.a*=min(float(p.PositionLife.w / p.Props.r),1);
	gl_Position=vec4(VP*dvec4(p.PositionLife.xyz,1));
	dvec4 vpos=V*dvec4(p.PositionLife.xyz,1);
	dvec4 s = P*dvec4(p.SpeedSize.w,p.SpeedSize.w,vpos.z,1);
	s/=s.w;
	Size=vec4(s);
	Index = float(p.Props.w);
	Life = float(p.PositionLife.w);

	p.PositionLife.xyz += p.SpeedSize.xyz * TimeDelta;
	p.SpeedSize.xyz += p.ForceRotation.xyz * TimeDelta;
	p.SpeedSize.w += p.Props.y * TimeDelta;
	p.PositionLife.w = max(p.PositionLife.w - TimeDelta, 0);

	Particles[gl_VertexID] = p;
}