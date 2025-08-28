#version 330 core

in vec3 WorldPos;

out vec4 FragColor;

uniform samplerCube environmentCubeMap;

void main() {		
    vec3 envColor = texture(environmentCubeMap, WorldPos).rgb;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
}