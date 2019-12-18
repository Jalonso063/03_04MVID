#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D tex_1;
uniform sampler2D tex_2;

vec2 InvUV = vec2(UV.x, UV.y * -1);

void main() {
    FragColor = texture(tex_1, InvUV);
}