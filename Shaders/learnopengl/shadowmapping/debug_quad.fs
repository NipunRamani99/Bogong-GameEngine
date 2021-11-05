#version 330 core
in VS_OUT{
    vec2 Pos;
    vec2 TexCoord;
} vs_out;
out vec4 FragColor;
uniform sampler2D outputTexture;
void main(){
    FragColor = texture(outputTexture, vs_out.TexCoord);
}