#version 460 core
in vec4 vert_v;
in vec3 vert_vn;
in vec2 vert_vt;
layout (location = 0) out vec4 color;
uniform uniforms
{
	vec3 eye_direction;
	vec3 light_pos;
	vec3 light_color;
	vec3 ambient;
	float diffuse;
	float specular;
	float shininess;
	float strength;
};
void main()
{
	vec4 vs_v_color = {1.0f, 1.0f, 1.0f, 1.0f};
	vec3 direction = light_pos - vert_v.xyz;
	float len = length(direction);
	direction = direction / len;
	vec3 half_vector = normalize(direction + eye_direction);
	float diffuse_ = max(0.0, dot(normalize(vert_vn), direction));
	float specular_ = max(0.0, dot(normalize(vert_vn), half_vector));

	if (diffuse == 0.0)
	{
		specular_ = 0.0;
	}
	else
	{
		specular_ = pow(specular_, shininess);
	}

	vec3 scattered_light = ambient + light_color * diffuse_;
	vec3 reflected_light = light_color * specular_ * strength;

	vec3 rgb = (-len / 100.0f + 1) * min(vs_v_color.rgb * scattered_light + reflected_light, vec3(1.0f));
	color = vec4(rgb, vs_v_color.a);
}
