#version 400 core
const float PI = 3.14159265358979323846;
//global data
float ka = 0.5;
float kd = 0.5;
float ks = 0.5;

out vec4 fragColor;
in vec4 Position;
in vec4 direction;
//in vec2 uv;

//Do Raytracing in here
uniform mat4 cam2world;
uniform vec4 eye;
uniform vec2 uResolution;
const int NUM_SPHERES = 7;
uniform mat4 Spheres[NUM_SPHERES];
uniform mat4 cubeTrans;

struct Material{
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

const int NUM_LIGHTS = 6;
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
    float t;
    vec4 normal;
    mat4 matTransformation;
};

struct Camera
{
    vec3 position;
    vec3 direction;
    float zoom;
} camera;

Material getmat(){
    Material color;
    color.ambient = vec4(0.50754f, 0.50754f, 0.50754f, 1.0f);
    color.diffuse = vec4(0.19225f, 0.19225f, 0.19225f, 1.0f);
    color.specular = vec4(0.508273f, 0.508273f, 0.508273f, 1.0f);
    color.shininess = 20.f;
    return color;
}

Light getLight0(){
    Light light;
    light.position = vec4(100.f, 100.f, 0.f, 1.f);
    light.color = vec4(0.1f, 0.1f, 0.1f, 1.f);
    light.constant =  1.f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    return light;
}

Light getLight1(){
    Light light;
    light.position = vec4(0.f, 100.f, 100.f, 1.f);
    light.color = vec4(0.1f, 0.1f, 0.1f, 1.f);
    light.constant =  1.f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    return light;
}

Light getLight2(){
    Light light;
    light.position = vec4(0.f, 100.f, 0.f, 1.f);
    light.color = vec4(0.5f, 0.5f, 0.5f, 1.f);
    light.constant =  1.f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    return light;
}

Light getLight3(){
    Light light;
    light.position = vec4(-100.f, -100.f, 0.f, 1.f);
    light.color = vec4(0.1f, 0.1f, 0.1f, 1.f);
    light.constant =  1.f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    return light;
}

Light getLight4(){
    Light light;
    light.position = vec4(0.f, -100.f, -100.f, 1.f);
    light.color = vec4(0.1f, 0.1f, 0.1f, 1.f);
    light.constant =  1.f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    return light;
}

Light getLight5(){
    Light light;
    light.position = vec4(0.f, -100.f, 0.f, 1.f);
    light.color = vec4(0.3f, 0.3f, 0.3f, 1.f);
    light.constant =  1.f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    return light;
}

miscData intersectSphere(vec4 d, vec4 e, float minT)
{
    miscData data;
    data.intersects = false;
    data.intersectW = vec4(0.f);
    data.matTransformation = mat4(0.f);
    data.normal = vec4(0.f);
    data.normalW = vec4(0.f);
    data.t = minT;
    float a = pow(d.x, 2.0f) + pow(d.y, 2.0f) + pow(d.z, 2.0f);
    float b = 2.0f * (e.x*d.x  + e.y*d.y +  e.z*d.z);
    float c = pow(e.x, 2.0f) + pow(e.y, 2.0f) + pow(e.z, 2.0f) - 0.25f;
    float disc = pow(b,2.f) - (4*a*c);
    if(disc>=0){
        float tTemp = 0;
        float tPlus = (-b + sqrt(disc))/ (2*a);
        if(tPlus < minT && tPlus > 0){
            tTemp = tPlus;
            data.intersects = true;
            vec4 intersect1 = e+tTemp*d;
            data.normal = normalize(vec4(2*intersect1.x, 2*intersect1.y, 2*intersect1.z, 0));
        }
        if(disc>=0){
            float tMinus = (-b - sqrt(disc))/ (2*a);
            if(tMinus < minT && tMinus > 0){
                tTemp = tMinus;
                data.intersects = true;
                vec4 intersect2 = e+tMinus*d;
                data.normal = normalize(vec4(2*intersect2.x, 2*intersect2.y, 2*intersect2.z, 0));
            }
        }
        if(data.intersects){
            data.t = tTemp;
        }
    }
    return data;
}

miscData intersect(vec4 d, vec4 e) {
    miscData data;
    data.t = 100000.f;
    data.normal = vec4(0.f);
    data.intersects = false;
    for (int i = 0; i < NUM_SPHERES; i++){ // for each Sphere
        mat4 matTrans = Spheres[i];
        miscData tempData;
        tempData = intersectSphere(inverse(matTrans)*d, inverse(matTrans)*e, data.t);
        if(tempData.intersects){

            if(tempData.t < data.t){
                data.t = tempData.t;
                data.normal = tempData.normal;
                data.intersects = true;
                data.matTransformation = matTrans;
            }

        }
    }

    if(data.intersects){
        data.intersectW = e + data.t * d;
        data.normalW = normalize(inverse(data.matTransformation) * data.normal);
    }
    return data;
}

vec4 calculateLighting(vec4 intersectW, vec4 d, vec4 normalW){
    vec4 color;
    Material silver = getmat();
    vec4 diffuseColor = kd*silver.diffuse;
    vec4 specularColor = ks*silver.specular;
    vec4 ambientColor = ka*silver.ambient;
    for(int i = 0; i<NUM_LIGHTS; i++){
        //lighting stuff
        Light light;
        if(i == 0){
            light = getLight0();
        }
        else if(i == 1){
            light = getLight1();
        }
        else if(i == 2){
            light = getLight2();
        }
        if(i == 3){
            light = getLight3();
        }
        else if(i == 4){
            light = getLight4();
        }
        else if(i == 5){
            light = getLight5();
        }
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

        bool shadows = true;
        if(shadows){
            vec4 p = intersectW+0.0001f*normalW;
            vec4 tempnormal;
            miscData d = intersect(direction, p);
            if(!d.intersects){
                color += ambientColor +  light.color*attenuation * (diffuseColor * NL) + (specularColor*pow(refdot,silver.shininess));
            }
        }
        else{
                color += ambientColor +  light.color*attenuation * (diffuseColor * NL) + (specularColor*pow(refdot,silver.shininess));
        }
    }
    return color;
}



vec4 raytrace(vec4 d, vec4 e){
    vec4 color = vec4(0.f);
    miscData data = intersect(d, e);
    if(data.intersects){
        color += calculateLighting(data.intersectW, d, data.normalW);
    } else {
        color = vec4(0.f, 0.f, 0.f, 1.f);
    }
    int depth = 1;
    miscData curData = data;
    vec4 reflectionColor = vec4(0.f);
    for(int i = 0; i < depth; i++){
        vec4 reflection = reflect(d, curData.normalW);
        miscData tempreflection = intersect(reflection, curData.intersectW+0.0001f*curData.normalW);
        if(tempreflection.intersects){
            reflectionColor+=calculateLighting(tempreflection.intersectW, reflection, tempreflection.normalW);
        }
        curData = tempreflection;
    }
    color += reflectionColor;
    color.x = clamp(color.x, 0.0f, 1.0f);
    color.y = clamp(color.y, 0.0f, 1.0f);
    color.z = clamp(color.z, 0.0f, 1.0f);
    color = vec4(color.x, color.y, color.z, 1);
    return color;
}

void main(){
    float x = Position.x;
    float y = Position.y;
    float h = uResolution.y;
    float w = uResolution.x;
    float heightAdiv2 = tan(radians(45.0f/2.0f));
    float widthAngle = 2 * atan(w*heightAdiv2, h);
    float u = 2*tan(widthAngle/2.0f);
    float v = 2*heightAdiv2;

    vec4 viewplane = vec4(u*x, v*y, -1.f, 1.f);
    vec4 d = viewplane-eye;
    d = normalize(cam2world*d);
    vec4 e = cam2world * eye;
    vec4 color = raytrace(d,e);

    fragColor = color;
}
