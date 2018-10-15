uniform sampler3D CloudDensityMap;
uniform sampler3D CloudNoiseMap;
uniform float Time;

const float SampleCount0 = 30;
const float SampleCount1 = 80;
const int SampleCountL = 10;
const vec3 NoiseFreq1 = vec3(1.0/4,16,1.0/4)*12e-6;
const float NoiseFreq2 = 140e-6;
const float NoiseBias = 0.6;
const vec3 Scroll1 = vec3(0.0003, 0.0024, 0.0018);
const vec3 Scroll2 = vec3(0.0003, 0.0015, 0.0009);
const float Altitude0 = 1500;
const float Altitude1 = 3500;
const float FarDist = 30000;
const float Scatter = 0.02;
const float HGCoeff = 0.1;
const float Extinct = 0.01;

const float iSun=22.0;                           // intensity of the sun
const float rPlanet=6371e3;                         // radius of the planet in meters
const float rAtmos=6471e3;                        // radius of the atmosphere in meters
const vec3 kRlh=vec3(5.5e-6, 13.0e-6, 22.4e-6); // Rayleigh scattering coefficient
const float kMie=21e-6;                          // Mie scattering coefficient
const float shRlh=8e3;                           // Rayleigh scale height
const float shMie=1.2e3;                          // Mie scale height
const float g=0.758;                           // Mie preferred scattering direction
const float gg = g * g;
const float pRlh = 3.0 / (16.0 * PI) * 2;
const float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * 2) / (pow(1.0 + gg - 2.0 * g, 1.5) * (2.0 + gg));
const float sunAngularDiameterCos = 0.99983194915;

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

float UVRandom(vec2 uv)
{
        float f = dot(vec2(12.9898, 78.233), uv);
        return fract(43758.5453 * sin(f));
}

float HenyeyGreenstein(float cosine)
{
    float g2 = HGCoeff * HGCoeff;
    return 0.5 * (1 - g2) / pow(1 + g2 - 2 * HGCoeff * cosine, 1.5);
}

float Beer(float depth)
{
    return exp(-Extinct * depth);
}

float BeerPowder(float depth)
{
    return exp(-Extinct * depth) * (1 - exp(-Extinct * 2 * depth));
}

float SampleNoise(vec3 uvw)
{
        vec3 uvw1 = uvw * NoiseFreq1+Scroll1 * Time;
        vec3 uvw2 = uvw * NoiseFreq2+Scroll2 * Time;
        float n1 = textureLod(CloudDensityMap, uvw1, 0).r;
        float n2 = textureLod(CloudNoiseMap, uvw2, 0).r;
        float n = n1 + n2;
        n = max(n+NoiseBias,0);
        float h = (uvw.y - Altitude0)/(Altitude1 - Altitude0);
        n*=(1-exp(-10*h))*exp(-3*h);
		return n;
}

float MarchLight(vec3 pos, vec3 light, float rand)
{
        float lngth = (Altitude1 - pos.y) / light.y;
        lngth=min(lngth,Altitude1-Altitude0);
		float stride = lngth / SampleCountL;
        pos += light * stride * rand;
        float depth = 0;
        for (int s = 0; s < SampleCountL; s++)
        {
            depth += SampleNoise(pos) * stride;
            pos += light * stride;
        }
        return BeerPowder(depth);
}

vec4 clouds(vec3 pos, vec3 dir, vec3 sunDir){
    int samples = int(mix(SampleCount1, SampleCount0, dir.y));
    float dist0; 
    float dist1; 
    if(abs(dir.y)<=0.01){
	dist0 = 0;
    dist1 = FarDist;
    }else{
	dist0 = clamp((Altitude0-pos.y) / dir.y,0,FarDist);
    dist1 = clamp((Altitude1-pos.y) / dir.y,0,FarDist);
    }
	float dist = min(abs(dist1 - dist0),FarDist);
    //float stride = dist / samples;
	float delta = 2*dist / (samples*(samples+1));
	float stride=delta;
	float hg = HenyeyGreenstein(dot(dir, sunDir));
    vec2 uv = gl_FragCoord.xy*1000 + vec2(Time);
    float offs = UVRandom(uv) * abs(dist1 - dist0) / samples;
    vec3 cpos = pos + dir * (min(dist0,dist1) + offs);
    vec3 acc = vec3(0);
    float depth = 0;
    for (int s = 0; s < samples; s++)
    {
        float n = SampleNoise(cpos);
        if (n > 0)
        {
            float density = n * stride;
            float rand = UVRandom(uv + vec2(s + 1));
            float scatter = density * Scatter * hg * MarchLight(cpos, sunDir, rand * 0.5);
			
			float halfSunDist=rsi(cpos+vec3(0,rPlanet,0),sunDir,rAtmos).y;
            float OdRlh = exp(-cpos.y / shRlh) * halfSunDist;
            float OdMie = exp(-cpos.y / shMie) * halfSunDist;
			vec3 Fex = exp(-(pRlh * kRlh * OdRlh + pMie * kMie * OdMie));
			
			acc += Fex * iSun * scatter * BeerPowder(depth);
            depth += density;
        }
        cpos += dir * stride;
		stride+=delta;
    }
	vec4 result=vec4(max(acc,0),Beer(depth));
	float distFactor=clamp(min(dist0,dist1) / FarDist,0,1);
    return mix(result,vec4(0,0,0,1),distFactor);
}