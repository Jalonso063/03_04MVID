#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);

mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

void main() {

    vec3 diffuse = texture(screenTexture, uv.st).rgb;
    mat3 A;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            vec3 texel  = texelFetch(screenTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
            A[i][j] = length(texel); 
        }
    }

    float gx = dot(sx[0], A[0]) + dot(sx[1], A[1]) + dot(sx[2], A[2]); 
    float gy = dot(sy[0], A[0]) + dot(sy[1], A[1]) + dot(sy[2], A[2]);

    float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
    FragColor = vec4(diffuse - vec3(g), 1.0);
   
}
