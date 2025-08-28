#version 330 core

in vec3 localPos;

out vec4 FragColor;
  
uniform samplerCube environmentMap;
  
void main() {
    vec3 envColor = texture(environmentMap, localPos).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));
  
    FragColor = vec4(envColor, 1.0);
}