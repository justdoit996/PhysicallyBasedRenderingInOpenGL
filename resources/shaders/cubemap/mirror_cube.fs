#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube cube_map;

void main()
{             
    float refractive_index = 1.52;
    vec3 I = normalize(Position - cameraPos);
    // Reflection
    // vec3 R = reflect(I, normalize(Normal));

    // Refraction
     vec3 R = refract(I, normalize(Normal), 1.0 / refractive_index);
    FragColor = vec4(texture(cube_map, R).rgb, 1.0);
}