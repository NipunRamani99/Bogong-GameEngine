#version 420 core

out vec4 fragColor;
in vec2 TexCoords;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 samples[64];
uniform float screenWidth;
uniform float screenHeight;
const vec2 noiseScale = vec2(screenWidth/4.0, screenHeight/4.0);
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

void main() {
    vec3 fragPos   =  ( view * vec4(texture(gPosition, TexCoords).xyz,1.0) ).xyz;
    vec3 normal    = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = normalize(cross(normal, tangent));
    mat3 TBN       = mat3(tangent, bitangent, normal);
    float occlusion = 0.0;
    
    for(int i = 0; i < kernelSize; i++)
    {
        // get sample position
        vec3 samplePos = TBN * samples[i]; // from tangent to view-space
        samplePos =  fragPos + samplePos * radius; 
        vec4 offset = vec4(samplePos, 1.0);
        offset      = projection  * offset;    // from view to clip-space
        offset.xyz /= offset.w;               // perspective divide
        offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  
        float sampleDepth = texture(gPosition, offset.xy).z; 
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0);
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    } 
    occlusion = 1.0 - (occlusion / kernelSize);
    fragColor = vec4(vec3(abs(occlusion)),1.0);  
}