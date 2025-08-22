#version 330 core

// in
in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

// out
out vec4 FragColor;

struct PointLight {
    vec3 Position;
    vec3 Color;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D diffuseTexture;

void main()
{
    vec3 texture_color = texture(diffuseTexture, TexCoords).rgb;
    vec3 result = vec3(0.0);

    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        vec3 lightDir = normalize(pointLights[i].Position - FragPos);
        float diffuse = max(dot(lightDir, Normal), 0.0);
        vec3 curr = pointLights[i].Color * texture_color * diffuse;
        float distance = length(FragPos - pointLights[i].Position);
        curr *= 1.0 / (distance * distance);
        result += curr;
    }
    FragColor = vec4(result, 1.0);
}