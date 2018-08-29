const vec3 waterAbsorbtionK=vec3(0.24,0.2,0.2);

vec3 AbsorbWater(vec3 inp, float dist){
    vec3 absorbtion=pow(vec3(10.0),-waterAbsorbtionK*dist);
    return inp*absorbtion;
}

bool IntersectRayPlane(vec3 rayOrigin, vec3 rayDirection, out vec3 intersectionPoint)
{
  if (rayDirection.y*sign(rayOrigin.y)>= 0 )
    return false;
  float s = rayOrigin.y / rayDirection.y;
  intersectionPoint = rayOrigin - s * rayDirection;
  return true;
}

vec3 ScatterWater(vec3 start, vec3 depth, lightData light){
    vec3 res=vec3(0);
    const int steps=10;
    vec3 step=depth/steps;
    float stepLength=length(step);
    vec3 cPos=start+step;
    for(int i=1; i<steps; i++)
    {
        vec3 endpoint;
        IntersectRayPlane(cPos,light.dir,endpoint);
        vec3 col=AbsorbWater(light.col,distance(cPos,endpoint));
        res+=stepLength*AbsorbWater(col,i*stepLength);
        cPos+=step;
    }
    return res;
}