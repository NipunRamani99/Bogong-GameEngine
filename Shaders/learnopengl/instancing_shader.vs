#version 330 core
layout( location = 0 ) in vec2 aPos;
layout( location = 1 ) in vec3 aColor;
layout( location = 2) in vec2 offset;
out vec3 FragColor;
uniform float time;
void main(){
  vec2 pos = aPos * (2.0+sin(time+gl_InstanceID))/2.0 ;
    gl_Position = vec4(pos + offset, 0.0, 1.0);
   FragColor = aColor;
}