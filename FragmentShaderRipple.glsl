#version 330 core

// Ouput data
in float lightIntensity;
in vec4 fragColor;
out vec4 color;
vec3 LightColor = vec3(1.0f,1.0f,1.0f);
void main()
{
	vec3 colort = vec3(fragColor)*(lightIntensity*LightColor);
    color = vec4(colort,1);
 
}