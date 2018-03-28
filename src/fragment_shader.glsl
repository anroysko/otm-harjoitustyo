#version 330 core

in vec3 fragment_color;
in vec2 fragment_uv;
out vec3 color;

uniform sampler2D texture_sampler;

void main() {
	// color = vec3(1, 0, 0);
	// color = fragment_color;
	color = texture(texture_sampler, fragment_uv).rgb;
}
