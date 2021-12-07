#version 330 core
//probably wont need, used in raytracing i think

in vec3 vertex;                 // The position of the vertex, in camera space
in vec3 vertexToLight;          // Vector from the vertex to the light
in vec3 vertexToCamera;            // Vector from the vertex to the eye
in vec3 eyeNormal;		// Normal of the vertex, in camera space

uniform samplerCube envMap;	// The cube map containing the environment to reflect
uniform vec4 ambient;		// The ambient channel of the color to reflect
uniform vec4 diffuse;		// The diffuse channel of the color to reflect
uniform vec4 specular;		// The specular channel of the color to reflect

uniform mat4 model;             // model matrix
uniform mat4 view;              // view matrix
uniform mat4 mvp;               // model view projection matrix

uniform float r0;		// The Fresnel reflectivity when the incident angle is 0
uniform float m;		// The roughness of the material

out vec4 fragColor;

void main()
{
    vec3 n = normalize(eyeNormal);
    vec3 l = normalize(vertexToLight);
    vec3 cameraToVertex = normalize(vertex); //remember we are in camera space!

    //TODO: fill the rest in
    float lambertTerm = dot(n, -l);
    float F = r0 + (1-r0)*pow((1-dot(n, -cameraToVertex)),5);
    float pi = 3.1415926535897932384626433832795;
    vec3 h = (-cameraToVertex+l)/2.0f;
    float alpha = acos(dot(n, h));
    float D = (1.0f/pow(m,2)*pow(cos(alpha),4))*exp(-(pow(tan(alpha)/m, 2)));
    float G = min(min(1.0f,2*((dot(n,h)*dot(n,-cameraToVertex))/dot(-cameraToVertex,h))), 2*((dot(n,h)*dot(n,l))/dot(-cameraToVertex,h)));
    float ks = (F/pi)*((D*G)/(pi * dot(n, -cameraToVertex) * dot(n,l)));
    vec4 objectColor = ambient+(lambertTerm*diffuse)+(ks*specular);
    vec3 reflected = vec3(inverse(view) * vec4(reflect(cameraToVertex, n), 0.0f));
    vec4 reflectColor = vec4(texture(envMap, reflected).rgb, 1.0);

    fragColor = mix(objectColor, reflectColor, F);
}
