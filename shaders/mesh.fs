#version 330 core

in vec3 f_pos;
in vec3 f_normal;

out vec4 FragColor;

void main() {
    vec3 light_pos = vec3(0.0f, 0.0f, 3.0f);
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
    vec3 m_color = vec3(1.0f, 0.5f, 0.2f);
    // ambient
    vec3 ambient = light_color * m_color;

    // diffuse
    vec3 light_dir = normalize(light_pos - f_pos);
    vec3 norm = normalize(f_normal);
    vec3 diffuse = light_color * max(dot(light_dir, norm), 0.0f);

    vec3 result = ambient * 0.2f + diffuse * 0.8f;
    FragColor = vec4(result, 1.0f);
}
