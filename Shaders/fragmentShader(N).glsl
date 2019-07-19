#version 330 core

// Ouput data
in vec4 fragColor;
out vec4 color;
uniform vec3 lightColor;
in vec3 Normals;
in vec3 fragPos;
uniform vec3 lightPos;
uniform vec3 viewPos;
void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    vec3 dir = normalize(Normals);
    vec3 lightDir = normalize(lightPos-fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);    
    vec3 reflectDir = reflect(-lightDir,dir);
    float diff = max(dot(dir, lightDir),0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 diffuse = diff*lightColor;
    vec3 specular = specularStrength*spec*lightColor;
    vec3 ambient = ambientStrength * lightColor;
    color = vec4(vec3(fragColor)*(ambient+diffuse+specular),1.0);
}
