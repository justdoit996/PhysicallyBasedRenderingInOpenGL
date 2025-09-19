#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform float bloomStrength = 0.004f;
uniform bool bloomEnabled;

vec3 bloom() {
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    return mix(hdrColor, bloomColor, bloomStrength); // linear interpolation
}

void main() {
    vec3 result = vec3(0.0);

    if (bloomEnabled) {
        result = bloom();
    } else {
        result = texture(scene, TexCoords).rgb;
    }

    // HDR tone mapping
    result = vec3(1.0) - exp(-result * exposure);
    result = result / (result + vec3(1.0));

    // Gamma correct
    const float gamma = 2.2;
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0);
}