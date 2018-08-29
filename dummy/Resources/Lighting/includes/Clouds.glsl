uniform sampler3D CloudDensityMap;
uniform sampler3D CloudNoiseMap;
uniform float Time;

const float SampleCount0 = 30;
const float SampleCount1 = 90;
const int SampleCountL = 16;
const float NoiseFreq1 = 3.1;
const float NoiseFreq2 = 35.1;
const float NoiseAmp1 = 5;
const float NoiseAmp2 = 1;
const float NoiseBias = -0.2;
const vec3 Scroll1 = vec3(0.0003, 0.0024, 0.0018);
const vec3 Scroll2 = vec3(0.0003, 0.0015, 0.0009);
const float Altitude0 = 1500;
const float Altitude1 = 3500;
const float FarDist = 30000;
const float Scatter = 0.08;
const float HGCoeff = 0.1;
const float Extinct = 0.002;


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
        const float baseFreq = 4e-6;
        vec3 uvw1 = uvw * NoiseFreq1 * baseFreq;
        vec3 uvw2 = uvw * NoiseFreq2 * baseFreq;
        uvw1 += Scroll1 * Time;
        uvw2 += Scroll2 * Time;
        float n1 = textureLod(CloudDensityMap, uvw1, 0).r;
        float n2 = textureLod(CloudNoiseMap, uvw2, 0).r;
        float n = n1 * NoiseAmp1 + n2 * NoiseAmp2;
        n = clamp(n + NoiseBias,0,1);
        float y = uvw.y - Altitude0;
        float h = Altitude1 - Altitude0;
        n *= smoothstep(0, h * 0.1, y);
        n *= smoothstep(0, h * 0.4, h - y);
        return n;
}

float MarchLight(vec3 pos, vec3 light, float rand)
{
        float stride = (Altitude1 - pos.y) / (light.y * SampleCountL);
        pos += light * stride * rand;
        float depth = 0;
        for (int s = 0; s < SampleCountL; s++)
        {
            depth += SampleNoise(pos) * stride;
            pos += light * stride;
        }
        return BeerPowder(depth);
}

vec3 clouds(vec3 albedo, vec3 pos, vec3 dir, vec3 sunDir, float sceneDepth){
    int samples = int(mix(SampleCount1, SampleCount0, dir.y));
    float dist0 = Altitude0 / dir.y;
    float dist1 = Altitude1 / dir.y;
    float stride = (dist1 - dist0) / samples;
    if (dir.y < 0.01 || dist0 >= FarDist || dist0>=sceneDepth) return albedo;
    float hg = HenyeyGreenstein(dot(dir, sunDir));
    vec2 uv = gl_FragCoord.xy*1000 + vec2(Time);
    float offs = UVRandom(uv) * (dist1 - dist0) / samples;
    vec3 cpos = pos + dir * (dist0 + offs);
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
            acc += vec3(1) * scatter * BeerPowder(depth);
            depth += density;
        }
        cpos += dir * stride;
    }
    acc += albedo*Beer(depth);
    //return vec3(depth/1000);
    return  mix(acc, albedo, clamp(dist0 / FarDist,0,1));
}