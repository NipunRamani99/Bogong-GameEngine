layout(location = 0) in vec2 auv_coords;

out vec2 pos;
void main() {
	gl_Position = vec4(auv_coords,0.1f,1.0f);
	pos = auv_coords;
}