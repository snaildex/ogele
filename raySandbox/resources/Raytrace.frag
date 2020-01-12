#version 430 core

in vec3 NearPos;
in vec3 FarPos;
in vec3 LookDir;
in vec2 UV;

layout(location = 0) out vec3 Result;

const int Rank = 32;
const float minDelta = 0.01;
const vec3 ldir=normalize(vec3(1,1,1));
uniform uint TriCount;

struct Triangle {
    vec4 Position[3];
    vec4 Normal[3];
    vec4 TriNormal;
    vec4 NormDot;
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
float Lambert(vec3 norm, vec3 dir) { return max(dot(norm,dir),0); }

vec3 Project(vec3 p, vec3 o, vec3 n) {
	vec3 t = p-o;
	vec3 d = dot(t, n) * n;
	return p - d;
}

float Height(vec3 p, vec3 o, vec3 n) {
	return dot(p-o, n);
}

void main()
{
    vec3 view = normalize(LookDir);
    Result=mix(vec3(1,0.7,0.5),vec3(0.8,0.9,1),pow(abs(view.y),0.5));

    float totalLength=length(FarPos-NearPos);
    int density=int(pow(2,Rank));
    float deltaLength=totalLength/density;
    deltaLength=max(deltaLength,minDelta);
    vec3 delta=view*deltaLength;
    float cdepth = totalLength;

    for(int t=0; t<TriCount; ++t)
    {
		vec3 curDelta=(NearPos-FarPos)/2;
		vec3 cpos=(NearPos+FarPos)/2;
        Triangle tri = Tris[t];
        vec3 ppos[3];
        vec3 fields;
        vec3 weights;
        bool hit=false;
        float curScene=0;
        float dir=0;
        for(int i=1; i<Rank; i++)
        {
            float height=dot(cpos-tri.Position[0].xyz,tri.TriNormal.xyz);
            for(int i=0; i<3; i++) {
                ppos[i]=tri.Position[i].xyz+tri.Normal[i].xyz/tri.NormDot[i]*height;
                //fields[i]=sdPlane(cpos,tri.Normal[i].xyz,tri.Position[i].xyz);
            }
			weights=Barycentric(cpos,ppos[0],ppos[1],ppos[2]);
            vec3 triPos=tri.Position[0].xyz*weights.r+tri.Position[1].xyz*weights.g+tri.Position[2].xyz*weights.b;
			for(int i=0; i<3; i++) {
				vec3 t=Project(triPos,tri.Position[i].xyz,tri.Normal[i].xyz);
				fields[i]=height-Height(t,tri.Position[0].xyz,tri.TriNormal.xyz);
			}
            curScene=dot(fields,weights);
			vec3 curNorm=tri.Normal[0].xyz*weights.r+tri.Normal[1].xyz*weights.g+tri.Normal[2].xyz*weights.b;
			curDelta/=2;
            dir=sign(curScene);
            if(dot(curNorm,view)<0) dir = -1;
            cpos-=dir*curDelta;
        }
		float depth = distance(NearPos, cpos);
        hit=curScene<=deltaLength;
        vec3 norm=tri.Normal[0].xyz*weights.r+tri.Normal[1].xyz*weights.g+tri.Normal[2].xyz*weights.b;
        float b = min3(weights);
        float f=Lambert(norm,ldir);
        if(hit && b>=0 && depth < cdepth && dot(norm,view)>0) {
			cdepth=depth;
			Result=(vec3(1,1,1) + pow(max(cos(cpos*(2*3.14)*10),0),vec3(5)))*(0.3+0.2*f);
			//Result=vec3(1,1,1)*(0.3+0.7*f);
		}
    }
}