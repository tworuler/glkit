#version 330 core

in vec3 f_pos;
out vec4 FragColor;

void main() {
    if (f_pos.x == 0) {
        FragColor = vec4(0.3, 0.54, 0.15, 1.0);
    } else if (f_pos.y == 0) {
        FragColor = vec4(0.56, 0.24, 0.28, 1.0);
    } else {
        FragColor = vec4(0.3, 0.3, 0.3, 1.0);
    }
}
