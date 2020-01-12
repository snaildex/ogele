const float iSun=22.0;                           // intensity of the sun
const float rPlanet=6371e3;                         // radius of the planet in meters
const float rAtmos=6471e3;                        // radius of the atmosphere in meters
const vec3 kRlh=vec3(5.5e-6, 13.0e-6, 22.4e-6); // Rayleigh scattering coefficient
const float kMie=21e-6;                          // Mie scattering coefficient
const float shRlh=8e3;                           // Rayleigh scale height
const float shMie=1.2e3;                          // Mie scale height
const float g=0.758;                           // Mie preferred scattering direction
const float sunAngularDiameterCos = 0.99983194915;

#define iSteps 12
#define jSteps 6

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

vec3 atmosphere(vec3 view, vec3 eyePos, vec3 sunDir, vec3 albedo, float depth, float sky) {
    eyePos+=vec3(0,rPlanet+1000,0);
    // Calculate the step size of the primary ray.
    vec2 p = rsi(eyePos, view, rAtmos);
    if (p.x > p.y) return vec3(0,0,0);
    p.y = min(p.y, rsi(eyePos, view, rPlanet).x);
    float iStepSize = ( (sky>0.5) ? (p.y - p.x) : depth) / float(iSteps);
    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    vec3 totalRlh = vec3(0,0,0);
    vec3 totalMie = vec3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(view, sunDir);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = mix(1, 3.0 / (16.0 * PI) * (1.0 + mumu), sky);
    float pMie = mix(1, 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg)), sky);
    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        vec3 iPos = eyePos + view * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float jStepSize = rsi(iPos, sunDir, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            vec3 jPos = iPos + sunDir * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;

            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    vec3 Fex = exp(-(pRlh * kRlh * iOdRlh + pMie * kMie * iOdMie));
    float sundisk = smoothstep(sunAngularDiameterCos,sunAngularDiameterCos+0.00002,mu);
	vec3 sun = iSun * Fex  *sundisk;
    //vec3 sun = Fex  *sundisk;
    vec3 result=(pRlh * kRlh * totalRlh + pMie * kMie * totalMie)*iSun+sun*sky;
    result+=Fex*albedo;

    return result;
}