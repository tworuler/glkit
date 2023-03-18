#version 330 core

uniform mat4 mvp;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec3 f_pos;
out vec3 f_normal;

void main() {
    f_pos = pos;
    f_normal = normal;
    gl_Position = mvp * vec4(pos, 1.0);
}