#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 pos;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
}