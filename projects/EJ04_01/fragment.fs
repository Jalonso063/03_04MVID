#version 330 core

in vec2 UV;
in vec3 myColor;
out vec4 FragColor;

uniform sampler2D tex_1;

void main() {
    FragColor = mix(texture(tex_1, UV), vec4(myColor, 1.0), 0.7);
}