#version 400 core
#define PI 3.14159265358979323846
out vec4 fragColor;

//Do Raytracing in here
uniform mat4 worldSpace; //scale*view
uniform vec4 eye; //view*vec4(0,0,0,1)
uniform float width;
uniform float height;

struct t{
    float minT;
    vec4 normal;
};

t intersectSphere(vec4 d, vec4 eye)
{
    struct t mt;
    float a = pow(d.x, 2.0)f + pow(d.y, 2.0f) + pow(d.z, 2.0f);
    float b = 2.0f * (eye.x*d.x  + eye.y*d.y +  eye.z*d.z);
    float c = pow(eye.x, 2.0f) + pow(eye.y, 2.0f) + pow(eye.z, 2.0f) - 0.25f;
    float disc = pow(a,2.f) - (4*a*c);
    if(disc>=0){
        float tPlus = (-b + qrt(disc))/ (2*a);
        float tMinus = (-b - glm::sqrt(disc))/ (2*a);
        if(tPlus<0 && tMinus < 0){
            mt.minT = -1.f
            return mt;
        }
        else if (tPlus<0 && tMinus >= 0){
            mt.minT = tMinus;
            mt.normal = normalize(eye+tMinus*d);
            return mt;
        }
        else if (tPlus>=0 && tMinus < 0){
            mt.minT = tPlus;
            mt.normal = normalize(eye+tPlus*d);
            return mt;
        }
        else{
            mt.minT = min(tPlus, tMinus);;
            mt.normal = normalize(eye+mt.minT*d);
            return mt;
        }
    }
}

vec4 calculateLighting(vec4 intersectW, vec4 d, vec4 normalW){
    vec4 color;

    //number of lights?
    for(int i = 0; i<0; i++){
        //lighting stuff
    }

    color.x = clamp(color.x, 0.0f, 1.0f);
    color.y = clamp(color.y, 0.0f, 1.0f);
    color.z = clamp(color.z, 0.0f, 1.0f);
    color = vec4(color.x, color.y, color.z, 1);
    return color;
}

void main(){
    float x = gl_FragCoord.x;
    float y = height - gl_FragCoord.y;
    vec4 film = vec4(x * 2.0 / width - 1.f, 1.f - y* 2.0f / height, -1.f, 1.f);
    vec4 world = worldSpace*film;
    vec4 d = normalize(world-eye);

    struct t mT= intersectSphere(d,eye);
    vec4 intersectW;
    vec4 normalW;
    if(t != -1.f){
        intersectW = eye+mT.mt*d;
        //normalW = normalize(objectTransformation*mT.normal);
    }
    vec4 color = calculateLighting(intersectW, d, normalW);
    //add reflections;

    fragColor = color;

//    vec3 texColor = texture(tex, texc).rgb;
//    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
//    fragColor = vec4(color * texColor, 1);
}
