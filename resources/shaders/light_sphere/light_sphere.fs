#version 330 core

// out
out vec4 FragColor;

// uniforms
uniform vec3 light_color;

void main() {
    FragColor = vec4(light_color, 1.0);
}