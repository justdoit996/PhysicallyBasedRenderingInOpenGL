#version 330 core

// in
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// out
out vec4 FragColor;
  
// textures
uniform sampler2D albedoMap;

uniform vec3 cameraPos;

void main()
{
    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    FragColor = vec4(albedo, 1.0);
};
