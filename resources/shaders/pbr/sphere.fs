#version 330 core

// in
in vec3 FragPos;

// out
out vec4 FragColor;
  
// uniforms
uniform vec3 cameraPos;

void main()
{
    FragColor = vec4((sin(FragPos.x * 15.0) / 2.0) + 0.5, 0.55, 0.5, 1.0);
};
