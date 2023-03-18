#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec3 m_pos;
out vec3 m_normal;

void main() {
    vec4 m_pos4 = model * vec4(pos, 1.0);
    gl_Position = projection * view * m_pos4;
    m_pos = m_pos4.xyz;
    m_normal = vec3(model * vec4(normal, 0.0));
}