#version 400 core

//Do Raytracing in here
uniform mat4 cameraSpace;
uniform vec3 eye;
uniform float width;
uniform float height;

float intersectSphere()
{
//    float a = pow(, 2.0)f + pow(, 2.0f) + pow(, 2.0f);
//    float b = 2.0f * ( *  +  *  +  * );
//    float c = pow(, 2.0f) + pow(, 2.0f) + pow(, 2.0f) - 0.25f;
//    float disc = pow(a,2.f) - (4*a*c);
//    if(disc>=0){
//        float tPlus = (-b + qrt(disc))/ (2*a);
//        float tMinus = (-b - glm::sqrt(disc))/ (2*a);
//        if(tPlus<0 && tMinus < 0){
//            return -1.f;
//        }
//        else if (tPlus<0 && tMinus >= 0){
//            return tMinus;
//        }
//        else if (tPlus>=0 && tMinus < 0){
//            return tPlus;
//        }
//        else{
//            return min(tPlus, tMinus);
//        }
//    }
}

void main(){
//    vec3 texColor = texture(tex, texc).rgb;
//    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
//    fragColor = vec4(color * texColor, 1);
}
