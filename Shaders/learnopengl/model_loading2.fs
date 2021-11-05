#version 330 core
in VS_OUT {
    vec3 Pos;
    vec3 Normal;
    vec2 TexCoord;
} gs_out;
uniform sampler2D texture_diffuse1;
out vec4 FragColor;
void main() {
 FragColor = vec4(1.0,0.0,0.0,1.0);
}