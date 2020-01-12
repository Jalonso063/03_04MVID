#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aUV;
layout (location=2) in vec3 aNormal;

out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main() {
    UV = aUV;
    //gl_Position = proj * view * model * vec4(aPos, 1.0);  // gl_Position inicial
    
    gl_Position = proj * model * view * vec4(aPos, 1.0);

    // Lo que descubrimos al invertir las transformaciones de traslacion y rotacion es que el cubo rota sobre el eje de la vista y no del cubo en si
    // De esta manera se aprecia que el orden de aplicacion es 100% relevante


}