#version 430 core
in vec4 vs_v_color;
in vec3 normal;
layout (location = 0) out vec4 color;
layout (location = 4) uniform vec3 ambient;
layout (location = 5) uniform vec3 light_color;
layout (location = 6) uniform vec3 light_direction;
layout (location = 7) uniform vec3 half_vector;
layout (location = 8) uniform float shininess;
layout (location = 9) uniform float strength;
void main()
{
	float diffuse = max(0.0f, dot(normal, light_direction));
	float specular = max(0.0f, dot(normal, half_vector));

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

	vec3 rgb = min(vs_v_color.rgb * scattered_light + reflected_light, vec3(1.0f));
	color = vec4(rgb, vs_v_color.a);
}
