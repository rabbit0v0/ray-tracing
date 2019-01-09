#version 460 core
layout (location = 0) in vec4 v;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;
out vec4 vert_v;
out vec3 vert_vn;
out vec2 vert_vt;
uniform mat4 clip_model;
void main()
{
	gl_Position = clip_model * v;
	vert_v = gl_Position;
	vert_vn = vn;
	vert_vt = vt;
}
