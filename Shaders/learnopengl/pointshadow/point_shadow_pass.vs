#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 Normal;
    vec2 TexCoord;
    vec3 FragPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool reverse_normals = false;

void main() {
    vs_out.TexCoord = aTexCoord;
   if(reverse_normals) // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
        vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
    else
        vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.FragPos = vec3(model * vec4(aPos,1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}