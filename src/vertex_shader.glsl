#version 330 core
layout(location = 0) in vec3 vertex_position;

out vec3 fragment_color;

void main() {
	gl_Position = vec4(vertex_position, 1.0);
	fragment_color = vec3(1, 0, 0);
}
