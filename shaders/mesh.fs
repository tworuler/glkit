#version 330 core

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 color;

in vec3 m_pos;
in vec3 m_normal;

out vec4 FragColor;

void main() {
    // ambient
    vec3 ambient = light_color * color;

    // diffuse
    vec3 light_dir = normalize(light_pos - m_pos);
    vec3 norm = normalize(m_normal);
    float diff = max(dot(light_dir, norm), 0.0f);
    vec3 diffuse = light_color * diff * color;

    vec3 result = ambient * 0.2f + diffuse * 0.8f;
    FragColor = vec4(result, 1.0f);
}
