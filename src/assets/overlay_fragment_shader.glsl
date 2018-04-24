// Fragment Shader
// overlay_fragment_shader.glsl
#version 330 core

in vec2 fragment_uv;
in vec3 fragment_color;
out vec3 color;

uniform sampler2D texture_sampler;

void main() {
	color = texture(texture_sampler, fragment_uv).rgb;
	if (color.r > 0.5 || color.g > 0.5 || color.b > 0.5) {
		discard;
	}
	color = fragment_color;
}
