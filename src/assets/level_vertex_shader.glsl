// Vertex shader
// level_vertex_shader.glsl

#version 330 core
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec2 vertex_dxy;

uniform vec2 map_scale;
uniform float move_scale;
uniform vec2 player_pos;

out vec2 fragment_uv;

void main() {
	gl_Position = vec4((vertex_position - player_pos + vertex_dxy * move_scale) * map_scale, 0.0, 1.0);
	fragment_uv = vertex_uv;
}
