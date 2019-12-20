#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aUV;
layout (location=2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMat;

out vec3 normal;
out vec3 fragPos;

void main() {
    normal = normalMat * aNormal;
    fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}