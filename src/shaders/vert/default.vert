#version 330 core
layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec4 v_color;
out vec4 vs_v_color;
out vec3 vs_v_normal;
out vec4 vs_v_position;
uniform mat4 clip_model;
void main()
{
	gl_Position = clip_model * v_position;
	vs_v_color = v_color;
	vs_v_normal = v_normal;
	vs_v_position = v_position;
}
