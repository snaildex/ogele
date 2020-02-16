#version 430 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

layout(location = 0) out vec3 Result;

const int Rank = 12;
const float minDelta = 0.01;
const vec3 ldir=normalize(vec3(1,1,1));
uniform uint TriCount;

struct Triangle {
    vec4 Position[3];
    vec4 Normal[3];
    vec4 TriNormal;
    vec4 NormDot;
	vec4 Domain;
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
    // ray-sphere intersection that assumes
    // the sphere is centered at the origin.
    // No intersection when result.x > result.y
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b*b) - 4.0*a*c;
    if (d < 0.0) return vec2(1e5,-1e5);
    return vec2(
        (-b - sqrt(d))/(2.0*a),
        (-b + sqrt(d))/(2.0*a)
    );
}

//vec4 PhongField(Triangle tri, vec3 pos) {
//	vec3 ppos[3];
//    vec3 weights;
//	float height=dot(pos-tri.Position[0].xyz,tri.TriNormal.xyz);
//    for(int i=0; i<3; i++) ppos[i]=tri.Position[i].xyz+tri.Normal[i].xyz/tri.NormDot[i]*height;
//    weights=Barycentric(pos,ppos[0],ppos[1],ppos[2]);
//    //return vec4(weights,Height(pos,tri.Position[0].xyz,tri.TriNormal.xyz));
//	//vec3 tpos = tri.Position[0].xyz*weights.x+tri.Position[1].xyz*weights.y+tri.Position[2].xyz*weights.z;
//    vec3 norm = normalize(tri.Normal[0].xyz*weights.x+tri.Normal[1].xyz*weights.y+tri.Normal[2].xyz*weights.z);
//	vec3 center=(tri.Position[0].xyz+tri.Position[1].xyz+tri.Position[2].xyz)/3 + tri.TriNormal.xyz*0.0;
//	for(int i=0; i<3; i++) ppos[i]=mix(center,tri.Position[i].xyz,weights[i]);
//	//vec3 tpos = ppos[0]*weights.x+ppos[1]*weights.y+ppos[2]*weights.z;
//	//return vec4(weights,Height(pos,tpos,norm));
//    vec3 dir = normalize(cross(normalize(ppos[1]-ppos[0]),normalize(ppos[2]-ppos[0])));
//	return vec4(weights,Height(pos,ppos[0],dir));
//}

vec4 PhongField(Triangle tri, vec3 pos) {
	vec3 ppos[3];
    vec3 weights;
	float height=dot(pos-tri.Position[0].xyz,tri.TriNormal.xyz);
    for(int i=0; i<3; i++) ppos[i]=tri.Position[i].xyz+tri.Normal[i].xyz/tri.NormDot[i]*height;
    weights=Barycentric(pos,ppos[0],ppos[1],ppos[2]);
    float field;
	if(weights.x>weights.y && weights.x>weights.z) field=Height(pos,tri.Position[0].xyz,tri.Normal[0].xyz);
	if(weights.y>weights.x && weights.y>weights.z) field=Height(pos,tri.Position[1].xyz,tri.Normal[1].xyz);
	if(weights.z>weights.y && weights.z>weights.x) field=Height(pos,tri.Position[2].xyz,tri.Normal[2].xyz);
	return vec4(weights,field);
}

void main()
{
    vec3 view = normalize(LookDir);
    Result=mix(vec3(1,0.7,0.5),vec3(0.8,0.9,1),pow(abs(view.y),0.5));

    int density=int(pow(2,Rank));
    float totalLength=length(FarPos-NearPos);
    float deltaLength=totalLength/density;
    deltaLength=max(deltaLength,minDelta);
    vec3 delta=view*deltaLength;
    float cdepth = totalLength;

//	vec3 cpos = vec3(UV, 0);
//	cpos*=2;
//	cpos.xy-=vec2(0.5);
//	Triangle tri;
//	tri.Position[0]=vec4(0,0,0,0);
//	tri.Position[1]=vec4(1,0,0,0);
//	tri.Position[2]=vec4(0,0,1,0);
//	tri.Normal[0]=normalize(vec4(-1,1.5,0,0));
//	tri.Normal[1]=normalize(vec4(1,1.5,0,0));
//	tri.Normal[2]=normalize(vec4(0,1,0.5,0));
//	tri.TriNormal=vec4(0,1,0,0);
//	tri.NormDot[0]=dot(tri.Normal[0],tri.TriNormal);
//	tri.NormDot[1]=dot(tri.Normal[1],tri.TriNormal);
//	tri.NormDot[2]=dot(tri.Normal[2],tri.TriNormal);
	//vec4 f = PhongField(t,cpos);
	//Result=vec3(f.a,f.a,f.a);
	//if(max(f.x,f.y)>1 || f.a<0) Result=vec3(0.6,0.8,1.0);

    for(int t=0; t<TriCount; ++t)
    {
		Triangle tri = Tris[t];
        vec2 bounds = rsi(NearPos-tri.Domain.xyz,LookDir,tri.Domain.w);
		if(bounds.x>bounds.y) continue;
		vec3 near=NearPos+LookDir*bounds.x;
		vec3 far=NearPos+LookDir*bounds.y;
		totalLength=length(far-near);
		deltaLength=totalLength/density;
		deltaLength=max(deltaLength,minDelta);
		vec3 curDelta=(far-near)/2;
		vec3 cpos=(near+far)/2;
        bool hit=false;
        float curScene=1000;
		vec3 curNorm=vec3(0);
		vec3 weights=vec3(0);
        float dir=0;
        for(int i=1; i<Rank; i++)
        {
            vec4 t=PhongField(tri, cpos);
			curScene=t.a;
			weights=t.xyz;
			curNorm=tri.Normal[0].xyz*weights.r+tri.Normal[1].xyz*weights.g+tri.Normal[2].xyz*weights.b;
			curDelta/=2;
            dir=sign(curScene);
            if(dot(curNorm,view)<0) dir = -1;
            cpos-=dir*curDelta;
        }
		float depth = distance(NearPos, cpos);
        hit=abs(curScene)<=deltaLength;
        float b = min3(weights);
        //if(hit && b>=0 && depth < cdepth && dot(curNorm,view)>0) {
        //if(hit && b>=0 && depth < cdepth) {
        if(hit && b>=0) {
        	float f=Lambert(curNorm,ldir);
        	cdepth=depth;
			Result=(vec3(1,1,1) + pow(max(cos(cpos*(2*3.14)*10),0),vec3(5)))*(0.3+0.2*f);
			//Result=weights;
		}
    }
}