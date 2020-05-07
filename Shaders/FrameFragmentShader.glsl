in vec2 UV;
uniform sampler2D s;
out vec4 FragColor;
const float offset = 1.0 / 300.0;
void main() {
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset), // top-left
		vec2(0.0f, offset), // top-center
		vec2(offset, offset), // top-right
		vec2(-offset, 0.0f),   // center-left
		vec2(0.0f, 0.0f),   // center-center
		vec2(offset, 0.0f),   // center-right
		vec2(-offset, -offset), // bottom-left
		vec2(0.0f, -offset), // bottom-center
		vec2(offset, -offset)  // bottom-right    
		);
	float kernel[9] = float[](
		0.5,0.5,0.5,
		0.5,-2,0.5,
		0.5,0.5,0.5
		);
	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(s, UV.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];
	col*= vec4(1.0f, 26.0f / 255.0f, 0.0, 1.0f);
	FragColor = vec4(col, 1.0);
}