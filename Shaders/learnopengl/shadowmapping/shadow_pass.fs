#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float shadowCalculations(vec4 fragPosLightSpace){
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz/ fragPosLightSpace.w;
    // transform the NDC coordinates to the range [0,1]:
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    vec3 color   = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 ambient = vec3(0.3);
    vec3 diffuse = vec3(0.7);
    float shadow = shadowCalculations(fs_in.FragPosLightSpace);
    vec3 lighting = ambient + shadow*diffuse;
    lighting = color * lighting;
    FragColor = vec4(lighting, 1.0);
}