#version 330 core
in vec4 vs_v_color;
in vec3 vs_v_normal;
in vec4 vs_v_position;
layout (location = 0) out vec4 color;
uniform mtl
{
	vec3 ambient;
	vec3 light_color;
	vec3 light_direction;
	vec3 eye_direction;
	vec3 light_pos;
	float shininess;
	float strength;
};
void main()
{
	vec3 direction = light_pos - vs_v_position.xyz;
	float len = length(direction);
	vec3 half_vector = normalize(direction + eye_direction);
	float diffuse = max(0.0f, dot(vs_v_normal, normalize(direction)));
	float specular = max(0.0f, dot(vs_v_normal, half_vector));

	if (diffuse == 0.0)
	{
		specular == 0.0;
	}
	else
	{
		specular = pow(specular, shininess);
	}

	vec3 scattered_light = ambient + light_color * diffuse;
	vec3 reflected_light = light_color * specular * strength;

	vec3 rgb = (-len / 100.0f + 1) * min(vs_v_color.rgb * scattered_light + reflected_light, vec3(1.0f));
	color = vec4(rgb, vs_v_color.a);
}
