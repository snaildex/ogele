#version 430 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

layout(location = 0) out vec3 Result;

const float minDelta = 0.01;
const vec3 ldir=normalize(vec3(1,1,1));
uniform uint Rank = 12;
uniform uint TriCount;
uniform uint SphereNum;

struct Triangle {
	vec4 Position[3];
	vec4 Normal[3];
	vec4 TriNormal;
	vec4 NormDot;
	vec4 Domain;
	vec4 VertexData[3];
};

layout(std430, binding = 0) buffer MeshData { Triangle Tris[]; };

float sdPlane( vec3 p, vec3 n, vec3 o) { return dot(p-o,n); }

vec3 Barycentric(vec3 p, vec3 a, vec3 b, vec3 c)
{
    float u,v,w;
    vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float idenom = 1/(d00 * d11 - d01 * d01);
    v = (d11 * d20 - d01 * d21) * idenom;
    w = (d00 * d21 - d01 * d20) * idenom;
    u = 1.0f - v - w;
    return vec3(u,v,w);
}
float min3 (vec3 v) { return min (min (v.x, v.y), v.z); }
float max3 (vec3 v) { return max (max (v.x, v.y), v.z); }
float Lambert(vec3 norm, vec3 dir) { return max(dot(norm,dir),0); }

vec3 Project(vec3 p, vec3 o, vec3 n) {
	vec3 t = p-o;
	vec3 d = dot(t, n) * n;
	return p - d;
}

float Height(vec3 p, vec3 o, vec3 n) {
	return dot(p-o, n);
}

float Dist(vec3 p, vec3 d, vec3 o, vec3 n) {
	float h=Height(p,o,n);
	return h/dot(d,n);
}

vec2 rsi(vec3 r0, vec3 rd, float sr) {
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b*b) - 4.0*a*c;
    if (d < 0.0) return vec2(1e5,-1e5);
	return vec2(-b/(2.0*a), sqrt(d)/(2.0*a));
}

float SphereDist(vec3 pos, vec3 dir, vec4 sphere) {
	vec2 r = rsi(pos-sphere.xyz, dir, abs(sphere.w));
	return r.x-sign(sphere.w)*r.y;
}

float PlaneDist(vec3 pos, vec3 dir, vec3 planePos, vec3 planeNorm) {
	return dot(pos-planePos, planeNorm) / dot(planeNorm, -dir);
}

vec4 BlendSpheres(vec3 pos, vec3 norm, float c0, float c1, vec2 w) {
	float c = c0*w.x+c1*w.y;
	c = c>0 ? max(c,1e-6) : min(c,-1e-6);
	float r = 1/c;
	return vec4(pos-norm*r,r);
}

float AngleBetween(vec3 a, vec3 b) { return acos(dot(normalize(a), normalize(b))); }

vec4 MSIField(Triangle tri, vec3 pos) {
	vec3 ppos[3];
    vec3 weights;
	float height=Height(pos, tri.Position[0].xyz, tri.TriNormal.xyz);
	for(int i=0; i<3; i++) ppos[i]=tri.Position[i].xyz+tri.Normal[i].xyz/tri.NormDot[i]*height;
    weights=Barycentric(pos,ppos[0],ppos[1],ppos[2]);
	vec3 cw = clamp(weights, 0, 1);
	cw /= cw.x + cw.y + cw.z;
	//return vec4(weights, height);
	//vec3 triPos = tri.Position[0].xyz*weights.x+tri.Position[1].xyz*weights.y+tri.Position[2].xyz*weights.z;
	vec3 triPos = tri.Position[0].xyz*cw.x+tri.Position[1].xyz*cw.y+tri.Position[2].xyz*cw.z;
	vec3 dir = normalize(triPos - pos)*sign(height);
	
	vec2 w01;
	vec2 w12;
	vec2 w02;
	w01.x = clamp(AngleBetween(triPos - tri.Position[2].xyz, tri.Position[1].xyz - tri.Position[2].xyz) / tri.VertexData[2].z,0,1);
	w12.x = clamp(AngleBetween(triPos - tri.Position[0].xyz, tri.Position[2].xyz - tri.Position[0].xyz) / tri.VertexData[0].z,0,1);
	w02.x = clamp(AngleBetween(triPos - tri.Position[1].xyz, tri.Position[2].xyz - tri.Position[1].xyz) / tri.VertexData[1].z,0,1);
	w01.y = 1 - w01.x;
	w12.y = 1 - w12.x;
	w02.y = 1 - w02.x;
	vec4 s0 = BlendSpheres(tri.Position[0].xyz,tri.Normal[0].xyz,tri.VertexData[0].x,tri.VertexData[0].y,w12);
	vec4 s1 = BlendSpheres(tri.Position[1].xyz,tri.Normal[1].xyz,tri.VertexData[1].x,tri.VertexData[1].y,w02);
	vec4 s2 = BlendSpheres(tri.Position[2].xyz,tri.Normal[2].xyz,tri.VertexData[2].x,tri.VertexData[2].y,w01);
	
	vec3 dist;
	dist.x = SphereDist(pos, dir, s0);
	dist.y = SphereDist(pos, dir, s1);
	dist.z = SphereDist(pos, dir, s2);
	//return vec4(weights,dist[SphereNum]);
	return vec4(weights, dot(dist,weights));
}

vec4 PhongField(Triangle tri, vec3 pos) {
	vec3 ppos[3];
    vec3 weights;
	float height=Height(pos, tri.Position[0].xyz, tri.TriNormal.xyz);
	for(int i=0; i<3; i++) ppos[i]=tri.Position[i].xyz+tri.Normal[i].xyz/tri.NormDot[i]*height;
    weights=Barycentric(pos,ppos[0],ppos[1],ppos[2]);
	vec3 triPos = tri.Position[0].xyz*weights.x+tri.Position[1].xyz*weights.y+tri.Position[2].xyz*weights.z;
	vec3 dir = normalize(triPos-pos)*sign(height);
	for (int i = 0; i < 3; i++) ppos[i] = Project(triPos, tri.Position[i].xyz, tri.Normal[i].xyz);
	vec3 phPos = ppos[0] * weights[0] + ppos[1] * weights[1] + ppos[2] * weights[2];
	phPos = mix(triPos, phPos, 0.6);
	return vec4(weights, PlaneDist(pos, dir, phPos, -dir));
}

float ShadeGrid(vec3 pos){ return max3(pow(max(cos(pos*(2*3.14)*10),0),vec3(10)));}

void main()
{
    vec3 view = normalize(LookDir);
	Result=mix(vec3(1,0.7,0.5),vec3(0.8,0.9,1),pow(abs(view.y),0.5));

    int density=int(pow(2,Rank));
    float cdepth = length(FarPos-NearPos);

    for(int t=0; t<TriCount; ++t)
    {
		Triangle tri = Tris[t];
        vec2 bounds = rsi(NearPos-tri.Domain.xyz,view,tri.Domain.w);
		if(bounds.y < 0 || bounds.x - bounds.y < 0) continue;
		vec3 near=NearPos+view*(bounds.x-bounds.y);
		vec3 far=NearPos+view*(bounds.x+bounds.y);
		float totalLength=length(far-near);
		float deltaLength=max(totalLength/density,minDelta);
		vec3 curDelta=(far-near)/2;
		vec3 cpos=(near+far)/2;
        bool hit=false;
        float curScene=1000;
		vec3 curNorm=vec3(0);
		vec3 weights=vec3(0);
        float dir=0;
        for(int i=1; i<Rank; i++)
        {
            //vec4 t=PhongField(tri, cpos);
            vec4 t=MSIField(tri, cpos);
			curScene=t.a;
			weights=t.xyz;
			curNorm=tri.Normal[0].xyz*weights.r+tri.Normal[1].xyz*weights.g+tri.Normal[2].xyz*weights.b;
			curDelta/=2;
            dir=sign(curScene);
            dir=min(-sign(dot(curNorm,view)),dir);
			if(dir==0) dir=1;
			cpos+=dir*curDelta;
        }
		float depth = distance(NearPos, cpos);
        hit=abs(curScene)<=deltaLength;
		curNorm=tri.Normal[0].xyz*weights.r+tri.Normal[1].xyz*weights.g+tri.Normal[2].xyz*weights.b;	
        float b = min3(weights);
        if(hit && b>=0 && depth < cdepth && distance(cpos,far) > 1e-3) {
        //if(hit && b>=0 && depth < cdepth) {
        //if(b>=0 && depth < cdepth) {
        	float f=Lambert(curNorm,ldir);
        	cdepth=depth;
			float grid = ShadeGrid(cpos);
			//Result = weights;
			Result=vec3((0.5+grid*0.25)*f+0.25);
			//Result=vec3((0.75)*f+0.25);
			//Result=vec3(curScene);
		}
    }
}