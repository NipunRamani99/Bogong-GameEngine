#version 330 core
out vec4 FragColor;
in vec4 fragColor;  
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0) * fragColor;
}