#version 330 core

// in
in vec3 FragPos;
in vec3 Normal;

// out
out vec4 FragColor;
  
// uniforms
uniform vec3 cameraPos;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

struct PointLight {
    vec3 pos;

    // Phong shading
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool blinn;

    // Attenuation
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

// Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);


void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 result;

    // Directional light
    result += CalcDirLight(dirLight, norm, viewDir);

    // Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir) {
    // 1. ambient
    vec3 ambient = dirLight.ambient;

    // 2. diffuse
    vec3 diffuse = dirLight.diffuse * dot(-dirLight.direction, normal);;

    // 3. specular
    vec3 reflectDir = reflect(-dirLight.direction, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec * material.specular) * dirLight.specular;

    return vec3(ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir) {
    vec3 result;
    // (1) Blinn-Phong shading
    // (1a) Ambient
    vec3 ambient = material.ambient * pointLight.ambient;
    // (1b) Diffuse
    vec3 lightDir = normalize(pointLight.pos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * pointLight.diffuse;
    // (1b) Specular
    float spec;
    if (pointLight.blinn) {
        // Blinn-Phong shading
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    vec3 specular = (spec * material.specular) * pointLight.specular;

    // (2) Attenuation
    float dist = length(pointLight.pos - FragPos);
    float attenuation = 1.f / (pointLight.constant + pointLight.linear * dist +
        pointLight.quadratic * dist*dist);

    result += (ambient + diffuse + specular) * attenuation;
    return result;
}