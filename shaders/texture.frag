#version 330 core

in vec2 uv;
in vec4 direction;

uniform vec2 uResolution;
uniform sampler2D tex;

out vec4 fragColor;

const float PI = 3.14159265358979323846264;

bool inArea(float cX, float cY) {
    return (sqrt(cX*cX + cY*cY) < 80.0);
}

//void main() {
//    fragColor = vec4(uv.x, uv.y,
//                     0, 1);
<<<<<<< HEAD
//}
//What does this draw? (assume entire screen is rendered)
void main() {
    float cX = gl_FragCoord.x - uResolution.x/2.0;
    float cY = gl_FragCoord.y - uResolution.y/2.0;
=======
//    fragColor = vec4(gl_FragCoord.x /(uResolution.x),
//                     gl_FragCoord.y /(uResolution.y),
//                     0, 1);
//}
// What does this draw? (assume entire screen is rendered)
void main() {
    float cX = gl_FragCoord.x - uResolution.x/2.0f;
    float cY = gl_FragCoord.y - uResolution.y/2.0f;
>>>>>>> 4b22afeb59562f2239334d3fbbd72b61bca57d36
    if (inArea(cX, cY)) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
