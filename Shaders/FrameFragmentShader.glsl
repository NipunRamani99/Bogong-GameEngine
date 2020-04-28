in vec2 UV;
uniform sampler2D s;
out vec4 FragColor;
void main() {
	FragColor = texture(s, UV);
	float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;

	FragColor = vec4(average, 0.0, average, 1.0);
}