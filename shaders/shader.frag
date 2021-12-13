#version 400 core
#define PI 3.14159265358979323846
//global data
#define ka 0.5
#define kd 0.5
#define ks 0.5

out vec4 fragColor;

//Do Raytracing in here
uniform mat4 worldSpace; //scale*view
uniform vec4 eye; //view*vec4(0,0,0,1)
uniform float width;
uniform float height;

//need numSpheres
struct t{
    float mT;
    vec4 normal;
} mt[numSpheres];

struct Material {
    vec4 diffuseColor;
    vec4 specularColor;
    float shininess;
};

//struct Light {
//};

vec4 intersect(vec4 d, vec4 eye) {
    //need material
    int minTIndex = 0;
    for (int i = 0; i < 0; i++){ // for each Sphere
        intersectSphere(d,eye, i); //might need to add transformation for object space
        if(i!=0 && mt[i].mT != -1.0f && mt[i].mT < mt[minTIndex].mT){
            minTIndex = i;
        }
    }

    vec4 color;
    vec4 intersectW;
    vec4 normalW;
    if(mt[minTIndex].mT != -1.f){
        intersectW = eye+mt[minTIndex].mT*d;
        //normalW = normalize(objectTransformation*mT.normal);
    }
    vec4 color = calculateLighting(intersectW, d, normalW, mat);//need to get material somehow


    return color;
}

void intersectSphere(vec4 d, vec4 eye, int i)
{
    float a = pow(d.x, 2.0)f + pow(d.y, 2.0f) + pow(d.z, 2.0f);
    float b = 2.0f * (eye.x*d.x  + eye.y*d.y +  eye.z*d.z);
    float c = pow(eye.x, 2.0f) + pow(eye.y, 2.0f) + pow(eye.z, 2.0f) - 0.25f;
    float disc = pow(a,2.f) - (4*a*c);
    if(disc>=0){
        float tPlus = (-b + qrt(disc))/ (2*a);
        float tMinus = (-b - glm::sqrt(disc))/ (2*a);
        if(tPlus<0 && tMinus < 0){
            mt[i].mT = -1.f;
        }
        else if (tPlus<0 && tMinus >= 0){
            mt[i].mT = tMinus;
            mt[i].normal = normalize(eye+tMinus*d);
        }
        else if (tPlus>=0 && tMinus < 0){
            mt[i].mT = tPlus;
            mt[i].normal = normalize(eye+tPlus*d);
        }
        else{
            mt[i].mT = min(tPlus, tMinus);
            mt[i].normal = normalize(eye+mt.minT*d);
        }
    }
}

vec4 calculateLighting(vec4 intersectW, vec4 d, vec4 normalW, Material mat){
    vec4 color;
    Material material = mat;
    //glm::vec2 uv = calcUV(object, normal, intersectO);
    //    float ambientR = m_global.ka*material.cAmbient.x;
    //    float ambientG = m_global.ka*material.cAmbient.y;
    //    float ambientB = m_global.ka*material.cAmbient.z;
    //    glm::vec4 diffuseColor = m_global.kd*material.cDiffuse;
    //    glm::vec4 specularColor = m_global.ks*material.cSpecular;
    //    glm::vec3 textureColor = glm::vec3(0.f,0.f,0.f);
    //    float R = ambientR;
    //    float G = ambientG;
    //    float B = ambientB;
    //    float blend = 0.0f;
    float attenuation = 1.0f;
    vec4 diffuseColor = kd*material.cDiffuse;
    vec4 specularColor = ks*material.cSpecular;

    //    CS123SceneLightData L;

    //number of lights?
    for(int i = 0; i<0; i++){
        //lighting stuff
        L = m_sceneLight[i];
        vec4 direction;
        float NL = 0.0f;
        float refdot = 0.0f;
        float dist = distance(L.pos, intersectW);
        attenuation = glm::min(1.0f/(L.function.x+(L.function.y*dist)+(L.function.z*pow(dist, 2))), 1.0f);
        direction = glm::normalize(L.pos - intersectW);
        float dot = glm::dot(normalW, direction);
        NL = std::min(std::max(dot, 0.0f), 1.0f);
        vec4 ref = direction - 2.0f * normalW * NL;
        refdot = glm::dot(glm::normalize(ref), glm::normalize(d));
        refdot = std::max(0.0f, refdot);
        //        if(settings.useTextureMapping){
        //            uv = calcUV(object, normal, intersectO);
        //            textureColor = calcTextureColor(material, uv, tInd);
        //            if(textureColor != glm::vec3(-1.f, -1.f, -1.f)){
        //                blend = material.blend;
        //            }

        //            if(settings.useShadows){
        //                bool intersects = false;
        //                glm::vec4 p = intersectW+0.0003f*normalW;
        //                glm::vec4 tempnormal = glm::vec4();
        //                int tempInd = -1;
        //                //true tells traceRay that it is tracing for shadows
        //                traceRay(p, direction, intersects, true, tempInd, normalW, intersectW, tempnormal, intersectO);
        //                if(!intersects){
        //                    color += L.color*attenuation * (diffuseColor * NL) + (specularColor.x*glm::pow(refdot,material.shininess)));
        //                }
        //            }
        //            else{
        //                color += L.color*attenuation * (diffuseColor * NL) + (specularColor.x*glm::pow(refdot,material.shininess)));
        //            }
        //        }

        color += L.color*attenuation * (diffuseColor * NL) + (specularColor.x*glm::pow(refdot,material.shininess)));
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

    vec4 color = intersect(d,eye);
    //add reflections?

    fragColor = color;

    //    vec3 texColor = texture(tex, texc).rgb;
    //    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
    //    fragColor = vec4(color * texColor, 1);
}
