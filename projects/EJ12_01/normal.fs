#version 330 core

out vec4 FragColor;

in vec2 uv;
in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    int shininess;
};
uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main() {
    vec3 normal = vec3(texture(material.normal, uv));
    normal = normalize(normal * 2.0 - 1.0);

    vec3 albedo = vec3(texture(material.diffuse, uv));
    vec3 ambient = albedo * light.ambient;

    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * albedo * light.diffuse;

    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, uv)) * light.specular;

    vec3 phong = ambient + diffuse + specular;
    FragColor = vec4(phong, 1.0f);
}