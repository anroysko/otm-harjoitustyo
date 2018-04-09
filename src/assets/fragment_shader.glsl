#version 330 core

in vec2 fragment_uv;
out vec3 color;

uniform sampler2D texture_sampler;

void main() {
	color = texture(texture_sampler, fragment_uv).rgb;
	if (
		254.0 / 255.0 < color.r
	&&	1.0   / 255.0 > color.g
	&&	199.0 / 255.0 < color.b
	&&	201.0 / 255.0 > color.b
	&&	254.0 / 255.0 < color.r) {
		discard;
	}
}
