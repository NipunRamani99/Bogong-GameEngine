#version 330 core
out vec4 color;
in VS_OUT{
    vec3 Normal;
    vec2 TexCoord;
} vs_out;
uniform sampler2D texture_diffuse1;
void main(){
    color = texture(texture_diffuse1,vs_out.TexCoord); 
}