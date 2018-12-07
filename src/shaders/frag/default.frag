#version 330 core
in vec4 vs_v_color;
in vec3 vs_v_normal;
in vec4 vs_v_position;
layout (location = 0) out vec4 color;
uniform mtl
{
	vec3 ambient;
	float diffuse;
	float specular;
	float shininess;
	float strength;
};
uniform eye_and_light
{
	vec3 eye_direction;
	vec3 light_pos;
	vec3 light_color;
};
void main()
{
	vec3 direction = light_pos - vs_v_position.xyz;
	float len = length(direction);
	vec3 half_vector = normalize(direction + eye_direction);
	float f_specular;

	if (diffuse == 0.0)
	{
		f_specular == 0.0;
	}
	else
	{
		f_specular = pow(specular, shininess);
	}

	vec3 scattered_light = ambient + light_color * diffuse;
	vec3 reflected_light = light_color * f_specular * strength;

	vec3 rgb = (-len / 100.0f + 1) * min(vs_v_color.rgb * scattered_light + reflected_light, vec3(1.0f));
	color = vec4(rgb, vs_v_color.a);
}
