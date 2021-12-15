#version 330 core

layout(location = 0) in vec3 ObjectSpace_position;
layout(location = 1) in vec3 ObjectSpace_normal;

uniform mat4 model, view, projection;

out vec3 WorldSpace_position; // world-space position
out vec3 WorldSpace_normal;   // world-space normal

void main() {
    vec4 wp = (model * vec4(ObjectSpace_position, 1.0));
    vec4 wpn = (model * vec4(ObjectSpace_normal, 0.0));
    WorldSpace_position = wp.xyz;
    WorldSpace_normal = wpn.xyz;

    gl_Position = projection * view * vec4(WorldSpace_position, 1.0);
}
