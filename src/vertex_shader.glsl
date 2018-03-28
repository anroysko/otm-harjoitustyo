#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

out vec3 fragment_color;
out vec2 fragment_uv;

void main() {
	gl_Position = vec4(vertex_position, 1.0);
	fragment_color = vec3(vertex_uv.x, vertex_uv.y, 0);
	fragment_uv = vertex_uv;
}
