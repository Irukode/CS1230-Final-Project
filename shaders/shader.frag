#version 400 core
#define PI 3.14159265358979323846
//global data
float ka = 0.5;
float kd = 0.5;
float ks = 0.5;

out vec4 fragColor;

//Do Raytracing in here
uniform mat4 cam2world; //scale*view
uniform vec4 eye; //view*vec4(0,0,0,1)
uniform float width;
uniform float height;
uniform int numSpheres;
uniform vec3 position;
uniform vec3 Spheres[numSpheres];

struct Material{
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

struct Light {
    vec4 position;
    vec4 color;
    float constant;
    float linear;
    float quadratic;
};

struct miscData{
    vec4 intersectW;
    vec4 normalW;
    bool intersects;
};

Material silver = Material(vec4(0.19225f, 0.19225f, 0.19225f, 1.0f), vec4(0.50754f, 0.50754f, 0.50754f, 1.0f), vec4(0.508273f, 0.508273f, 0.508273f, 1.0f), 51.2f);
Light light = Light(vec4(10.f, 10.f, 10.f, 0.f), vec4(0.5f, 0.5f, 0.5f, 0.f), 1.f, 0.09f, 0.032f);

bool intersectSphere(vec4 d, vec4 eye, out float t, out vec4 normal)
{
    float a = pow(d.x, 2.0f) + pow(d.y, 2.0f) + pow(d.z, 2.0f);
    float b = 2.0f * (eye.x*d.x  + eye.y*d.y +  eye.z*d.z);
    float c = pow(eye.x, 2.0f) + pow(eye.y, 2.0f) + pow(eye.z, 2.0f) - 0.25f;
    float disc = pow(a,2.f) - (4*a*c);
    if(disc>=0){
        float tPlus = (-b + sqrt(disc))/ (2*a);
        float tMinus = (-b - sqrt(disc))/ (2*a);
        if(tPlus<0 && tMinus < 0){
            t = 100000.f;
            return false;
        }
        else if (tPlus<0 && tMinus >= 0){
            t = tMinus;
            normal = normalize(eye+t*d);
            return true;
        }
        else if (tPlus>=0 && tMinus < 0){
            t = tPlus;
            normal = normalize(eye+t*d);
            return true;
        }
        else{
            t = min(tPlus, tMinus);
            normal = normalize(eye+t*d);
            return true;
        }
    }
    return false;
}

//intersect cube
//bool intersectCube(vec4 eye, vec4 d){
//    bool intersection = false;

//    if(d.y!=0){
//        float t1 = (0.5f-eye.y)/d.y;
//        vec4 intersect1 = eye+(t1*d);
//        float t2 = (-0.5f-eye.y)/d.y;
//        vec4 intersect2 = eye+(t2*d);
//        if(abs(intersect1.x)<=0.5f && abs(intersect1.z) <= 0.5f){
//            if(t1>=0 && t1<t2){
//                c.mT = t1;
//                c.normal = vec4(0,1,0,0);
//            }
//        }

//        if(abs(intersect2.x)<=0.5f && abs(intersect2.z) <= 0.5f){
//            if(t2<t1 && t2>=0){
//                c.mT = t2;
//                c.normal = vec4(0,-1,0,0);
//            }
//        }
//    }

//    if(d.x!=0){
//        float t3 = (0.5f-eye.x)/d.x;
//        vec4 intersect3 = eye+(t3*d);
//        float t4 = (-0.5f-eye.x)/d.x;
//        vec4 intersect4 = eye+(t4*d);
//        if(abs(intersect3.y)<=0.5f && abs(intersect3.z) <= 0.5f){
//            if(t3<t4 && t3>=0){
//                c.mT = t3;
//                c.normal = vec4(1,0,0,0);
//            }
//        }

//        if(abs(intersect4.y)<=0.5f && abs(intersect4.z) <= 0.5f){
//            if(t4<t3 && t4>=0){
//                c.mT = t4;
//                c.normal = vec4(-1,0,0,0);
//            }
//        }
//    }

//    if(d.z!=0){
//        float t5 = (0.5f-eye.z)/d.z;
//        vec4 intersect5 = eye+(t5*d);
//        float t6 = (-0.5f-eye.z)/d.z;
//        vec4 intersect6 = eye+(t6*d);
//        if(abs(intersect5.y)<=0.5f && abs(intersect5.x) <= 0.5f){
//            if(t5<t6 && t5>=0){
//                c.mT = t5;
//                c.normal = vec4(0,0,1,0);
//            }
//        }

//        if(abs(intersect6.y)<=0.5f && abs(intersect6.x) <= 0.5f){
//            if(t6<t5 && t6>=0){
//                c.mT = t6;
//                c.normal = vec4(0,0,-1,0);
//            }
//        }
//    }
//}

miscData intersect(vec4 d, vec4 eye) {
    miscData data;
    float t = 100000.f;
    float t1;
    vec4 normal;
    vec4 normal1;
    for (int i = 0; i < numSpheres; i++){ // for each Sphere
        //intersectSphere(d,eye, i, ); //might need to add transformation for object space
        if(intersectSphere(d, eye, t1, normal1)){
            if(t1<t){
                t = t1;
                normal = normal1;
            }
        }
    }
    if(t != 100000.f){
        data.intersectW = eye+t*d;
        //data.normalW = normalize(objectTransformation*normal);
        data.intersects = true;
        //color = calculateLighting(intersectW, d, normalW);
    }
    return data;
}

vec4 calculateLighting(vec4 intersectW, vec4 d, vec4 normalW){
    vec4 color;
    vec4 diffuseColor = kd*silver.diffuse;
    vec4 specularColor = ks*silver.specular;
    vec4 ambientColor = ka*silver.ambient;
//        if(shadows){
//            bool intersects = false;
//            vec4 p = intersectW+0.0003f*normalW;
//            vec4 tempnormal;
//            intersect(d, p, true, intersects);
//            if(!intersects){
//                color = ambientColor +  light.color*attenuation * (diffuseColor * NL) + (specularColor*pow(refdot,silver.shininess));
//            }

//        }

    //lighting stuff
    float NL = 0.0f;
    float refdot = 0.0f;
    float dist = distance(light.position, intersectW);
    float attenuation = min(1.0f/(light.constant+(light.linear*dist)+(light.quadratic*pow(dist, 2))), 1.0f);
    vec4 direction = normalize(light.position - intersectW);
    float dotp = dot(normalW, direction);
    NL = min(max(dotp, 0.0f), 1.0f);
    vec4 ref = direction - 2.0f * normalW * NL;
    refdot = dot(normalize(ref), normalize(d));
    refdot = max(0.0f, refdot);

    color = ambientColor +  light.color*attenuation * (diffuseColor * NL) + (specularColor*pow(refdot,silver.shininess));

    color.x = clamp(color.x, 0.0f, 1.0f);
    color.y = clamp(color.y, 0.0f, 1.0f);
    color.z = clamp(color.z, 0.0f, 1.0f);
    color = vec4(color.x, color.y, color.z, 1);
    return color;
}



vec4 raytrace(vec4 d, vec4 eye){
    vec4 color;
    miscData data = intersect(d, eye);
    if(data.intersects){
        color = calculateLighting(data.intersectW, d, data.normalW);
    }

    //reflections

    return color;
}

void main(){
    float x = position.x;
    float y = position.y;
    vec4 world = cam2world;
    vec4 d = normalize(world-eye);

    vec4 color = raytrace(d,eye);

    //add reflections?

    //color = ka*silver.ambient +  kd*silver.diffuse + ks*silver.specular;
    fragColor = color;
}
