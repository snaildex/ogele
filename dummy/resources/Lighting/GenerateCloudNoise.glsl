#version 430 core

layout(local_size_x = 32, local_size_y = 1, local_size_z = 32) in;
layout(r16f) uniform image3D CloudNoiseMap;

const float Amplitude = 2;


float noise3D(vec3 p)
{
	return fract(sin(dot(p ,vec3(12.9898,78.233,126.7235))) * 43758.5453);
}

float worley3D(vec3 p)
{					 					 
	float r = 3.0;
    vec3 f = floor(p);
    vec3 x = fract(p);
	for(int i = -1; i<=1; i++)
	{
		for(int j = -1; j<=1; j++)
		{
			for(int k = -1; k<=1; k++)
			{
                vec3 q = vec3(float(i),float(j),float(k));
    			vec3 v = q + vec3(noise3D((q+f)*1.11), noise3D((q+f)*1.14), noise3D((q+f)*1.17)) - x;
    			float d = dot(v, v);
				r = min(r, d);
			}
		}
	}
    return sqrt(r);
}	

void main()
{
    vec3 pos=vec3(gl_GlobalInvocationID);
    float scale=0.08;
    float val=-worley3D(pos*scale)-worley3D(pos*scale*2);
	val*=Amplitude;
    imageStore(CloudNoiseMap,ivec3(gl_GlobalInvocationID),vec4(val));
}