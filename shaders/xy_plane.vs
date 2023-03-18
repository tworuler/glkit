#version 330 core

uniform mat4 mvp;
in vec3 pos;
out vec3 f_pos; 

void main() {
    f_pos = pos;
    gl_Position = mvp * vec4(pos, 1.0);
}