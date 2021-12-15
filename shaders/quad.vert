#version 330 core

layout(location = 0) in vec3 position;
layout(location = 5) in vec2 inUV;

uniform mat4 model, view, projection;
uniform vec4 camera;

out vec2 uv;
out vec4 direction;
out vec4 Position;

void main() {
    uv = inUV;
    gl_Position = vec4(position, 1.0);
    direction = inverse(view) * (gl_Position - camera);
    Position = gl_Position;
}
