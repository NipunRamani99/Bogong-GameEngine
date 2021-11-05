#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
    vec3 Normal;
    vec3 pos;
} gs_out;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, gs_out.TexCoords);
}