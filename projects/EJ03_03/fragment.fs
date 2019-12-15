#version 330 core

in vec3 myColor;
out vec4 FragColor;

uniform vec3 addColor;

void main() {
    FragColor = vec4(myColor + addColor, 1.0);
}