// Vertex Shader
// overlay_vertex_shader.glsl
#version 330 core
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_color;

uniform float camera_dy;

out vec2 fragment_uv;
out vec3 fragment_color;

void main() {
	gl_Position = vec4(vertex_position.x, vertex_position.y - camera_dy, 0.0, 1.0);
	fragment_uv = vertex_uv;
	fragment_color = vertex_color;
}
