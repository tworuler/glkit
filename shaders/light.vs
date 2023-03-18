#version 330 core

uniform mat4 mvp;

layout (location = 0) in vec3 pos;

void main() {
    gl_Position = mvp * vec4(pos, 1.0);
}