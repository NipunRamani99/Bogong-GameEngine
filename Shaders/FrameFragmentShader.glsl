in vec2 UV;
uniform sampler2D s;
out vec4 FragColor;
void main() {
	FragColor = texture(s, UV);
}