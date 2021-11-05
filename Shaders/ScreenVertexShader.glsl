#version 330 core
layout(location = 0) in vec2 auv_coords;

out VS_OUT{
    vec2 pos;
} vs_out;

void main() {
	gl_Position = vec4(auv_coords,0.10f,1.0f);
	vs_out.pos = auv_coords;
}