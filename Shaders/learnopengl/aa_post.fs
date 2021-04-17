#version 330 core
in vec2 TexCoords;
uniform sampler2D screenTexture;
out vec4 FragColor;
void main(){
    vec3 col = texture(screenTexture, TexCoords).rgb;
    float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    FragColor = vec4(vec3(grayscale),1.0);
}