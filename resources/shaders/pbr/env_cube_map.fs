#version 330 core

in vec3 WorldPos;

out vec4 FragColor;

uniform samplerCube environmentMap;

void main() {		
    // For testing and viewing other env textures e.g. prefilter
    // vec3 envColor = textureLod(environmentMap, WorldPos, 1.2).rgb; 
    vec3 envColor = texture(environmentMap, WorldPos).rgb;
    
    // This is now done in the framebuffer
    // HDR tonemap and gamma correct
    //envColor = envColor / (envColor + vec3(1.0));
    //envColor = pow(envColor, vec3(1.0/2.2)); 
    
    FragColor = vec4(envColor, 1.0);
}