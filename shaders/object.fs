#version 330 core

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 color;
uniform int render_mode;
uniform float near;
uniform float far;

in vec3 m_pos;
in vec3 m_normal;
in float depth_eye;

out vec4 FragColor;

vec4 calc_lighting() {
    // ambient
    vec3 ambient = light_color * color;

    // diffuse
    vec3 light_dir = normalize(light_pos - m_pos);
    vec3 norm = normalize(m_normal);
    float diff = max(dot(light_dir, norm), 0.0f);
    vec3 diffuse = light_color * diff * color;

    vec3 result = ambient * 0.2f + diffuse * 0.8f;
    return vec4(result, 1.0f);
}

vec4 calc_depth() {
    float depth = ( depth_eye - near) / (far - near);
    depth = 1.0f - depth;
    return vec4(vec3(depth), 1.0f);
}

void main() {
    if (render_mode == 0) {
        FragColor = calc_lighting();
    } else if (render_mode == 1) {
        FragColor = calc_depth();
    }
}
