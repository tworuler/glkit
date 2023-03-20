#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec3 m_pos;
out vec3 m_normal;
out float depth_eye;

void main() {
    vec4 m_pos4 = model * vec4(pos, 1.0);
    vec4 v_pos4 = view * m_pos4;
    vec4 p_pos4 = projection * v_pos4;
    m_pos = m_pos4.xyz;
    m_normal = vec3(model * vec4(normal, 0.0));
    depth_eye = -v_pos4.z;
    gl_Position = p_pos4;
}