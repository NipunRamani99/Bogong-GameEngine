#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
out vec3 fragPos;
void main() { 
    fragPos = normalize(aPos);
    vec4 pos =  projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
