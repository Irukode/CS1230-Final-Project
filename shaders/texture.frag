#version 330 core

in vec2 uv;
in vec4 direction;

uniform vec2 uResolution;
uniform sampler2D tex;

out vec4 fragColor;

const float PI = 3.14159265358979323846264;

bool inArea(float cX, float cY) {
    return (sqrt(cX*cX + cY*cY) < 100.0);
}

void main() {
    float cX = gl_FragCoord.x - uResolution.x/2.0f;
    float cY = gl_FragCoord.y - uResolution.y/2.0f;
    if (inArea(cX, cY)) {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        fragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
