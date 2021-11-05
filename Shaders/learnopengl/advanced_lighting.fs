#version 330 core
in VS_OUT{
    vec3 Pos;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;
out vec4 FragColor;
uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;
uniform bool gammaCorrection;
uniform float blinnPower;
uniform float specPower;
void main(){
    vec3 color = texture(texture_diffuse1, vs_out.TexCoord).rgb;
    //ambient
    vec3 ambient = 0.5 * color;
    //diffuse
    vec3 lightDir = normalize(lightPos - vs_out.Pos);
    vec3 normal = normalize(vs_out.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    //specular
    vec3 viewDir = normalize(viewPos - vs_out.Pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(blinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal,halfwayDir),0.0), blinnPower);
    }
    else{
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir),0.0), specPower);
    }
    if(gammaCorrection){
        float gamma = 2.2;
        diffuse = pow(diffuse, vec3(1.0/gamma));
    }
    vec3 specular = vec3(0.3) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}