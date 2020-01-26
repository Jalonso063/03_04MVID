#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

void main() { 

    vec3 tex = vec3(texture(screenTexture, uv));
    
    FragColor = vec4(tex, 1.0);
}
