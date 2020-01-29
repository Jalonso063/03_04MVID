#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 uv;


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    int shininess;
};
uniform Material material;

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};
uniform SpotLight light;

uniform vec3 viewPos;

uniform sampler2D depthMap;

#define NUMBER_SPOT_LIGHTS 2
uniform SpotLight spotLight[NUMBER_SPOT_LIGHTS];
in vec4 fragPosLighSpace[NUMBER_SPOT_LIGHTS];

float ShadowCalculation(vec4 fragPosLighSpace, float bias) {
    vec3 projCoords = fragPosLighSpace.xyz / fragPosLighSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    //float closestDepth = texture(depthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for (int y = -1; y <=1; ++y) {
            float pcf = texture(depthMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth - (bias / fragPosLighSpace.w) > pcf ? 1.0 : 0.0;
        }
	}
    shadow /= 9.0;

    if (projCoords.z > 1.0) {
     shadow = 0.0;
	}

    return shadow;
}

vec3 calcSpotLight(SpotLight light, vec4 fragPosLighSpace, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 albedoMap, vec3 specularMap) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant +
        light.linear * distance +
        light.quadratic * distance * distance);

    vec3 ambient = albedoMap * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * albedoMap * light.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * specularMap * light.specular;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
    float shadow = ShadowCalculation(fragPosLighSpace, bias);

    return (ambient + ((1.0 - shadow) * ((diffuse * intensity) + (specular * intensity)))) * attenuation;
}

void main() {
    vec3 albedo = vec3(texture(material.diffuse, uv));
    vec3 ambient = albedo * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * albedo * light.diffuse;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, uv)) * light.specular;

    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < NUMBER_SPOT_LIGHTS; ++i) {
        finalColor += calcSpotLight(spotLight[i],fragPosLighSpace[i], norm, viewDir, fragPos, albedo, specular);
	}

    FragColor = vec4(finalColor, 1.0f);
}