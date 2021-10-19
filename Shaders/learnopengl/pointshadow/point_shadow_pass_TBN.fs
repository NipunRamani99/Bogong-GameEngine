#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform samplerCube depth_cube_map;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

out vec4 fragColor;
// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float shadowCalculation(in vec3 fragPos) {
    vec3 fragToLight = fragPos - lightPos;
    //float closestDepth = texture(depth_cube_map, fragToLight).r;
    //closestDepth *= far_plane;
    float currentDepth = length(fragToLight);
   //float bias = 0.05;
   //float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depth_cube_map, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

void main() {
    vec3 normal = texture(texture_normal1, vs_out.TexCoords).rgb;
    normal = normalize(2.0 * normal - 1.0);
        vec3 color = texture(texture_diffuse1, vs_out.TexCoords).rgb;
    vec3 lightColor = vec3(0.3);
    vec3 ambient = vec3(0.3) * lightColor;
    vec3 lightDir = normalize(vs_out.TangentLightPos - vs_out.TangentFragPos);
      vec3 viewDir = normalize(vs_out.TangentViewPos - vs_out.TangentFragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float diff = max(dot(lightDir, normal), 0.);
    float spec = pow(max(dot(normal, halfDir), 0.0), 32.0);
 
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;
    float shadow = shadowCalculation(vs_out.FragPos);
    vec3 lighting = (ambient + (1.0-shadow)*(diffuse+specular)) * color;
    fragColor = vec4(lighting, 1.0);
}
