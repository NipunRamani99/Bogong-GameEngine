#version 330 core

in VS_OUT {
    vec3 Normal;
    vec2 TexCoord;
    vec3 FragPos;
} fs_in;

uniform sampler2D diffuse_texture;
uniform samplerCube depth_cube_map;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

out vec4 fragColor;

float shadowCalculation(in vec3 fragPos) {
    vec3 fragToLight = fragPos - lightPos;
    float closestDepth = texture(depth_cube_map, fragToLight).r;
    closestDepth *= far_plane;
    float currentDepth = length(fragPos - lightPos);
    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

void main() {
    
    vec3 color = texture(diffuse_texture, fs_in.TexCoord).rgb;
    vec3 lightColor = vec3(0.3);
    vec3 ambient = 0.3 * color;
    
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal),0.);
    vec3 diffuse = diff * lightColor;
    
    vec3 eyeDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(eyeDir + lightDir);
    float spec = pow(max(dot(normal,halfwayDir),0.),64.0);
    vec3 specular = spec * lightColor;

    float shadow = shadowCalculation(fs_in.FragPos);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    fragColor = vec4(vec3(lighting), 1.0);
 }

