#version 430 core
layout (location = 0) in vec4 v_position;
layout (location = 1) in vec4 v_color;
out vec4 vs_v_color;
out vec3 normal;
out vec4 vs_v_position;
layout (location = 0) uniform mat4 clip_model;
void main()
{
	gl_Position = clip_model * v_position;
	vs_v_color = v_color;
	normal = normalize(v_position.xyz);
	vs_v_position = v_position;
}
