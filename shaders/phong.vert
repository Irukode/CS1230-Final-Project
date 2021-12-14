#version 330 core

//layout(location = 0) in vec3 position;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 perspective;

//void main() {
//    gl_Position = vec4(position, 1.0);
//    // TODO: Add 0.5 to gl_Position.y (Task 2)
//    //gl_Position.y += 0.5f;
//    // TODO: Transform gl_Position using the model matrix uniform (Task 3)
//    //gl_Position = model * gl_Position;
//    // TODO: Transform the position using all three matrix uniforms. (Task 4)
//    gl_Position = perspective * view * model * gl_Position;
//}

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
