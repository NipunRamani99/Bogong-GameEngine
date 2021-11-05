#version 330 core
in VS_OUT {
    vec3 Pos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;
uniform sampler2D texture_diffuse1;
out vec4 FragColor;
void main() {
 FragColor = texture(texture_diffuse1, vs_out.TexCoord);
}