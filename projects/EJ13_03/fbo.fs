#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

uniform vec2 offset;

void main() {

float kernel[9] = float[] (
    0.028532, 0.067234,	0.124009,
    0.179044, 0.20236, 0.179044,
    0.124009, 0.067234, 0.028532);

    vec3 tex = vec3(0.0);
    for (int i = -4; i <= 4; ++i) {
        tex += vec3(texture(screenTexture, uv + i*offset)) * kernel[i+4];
    }

    FragColor = vec4(tex, 1.0);

}
