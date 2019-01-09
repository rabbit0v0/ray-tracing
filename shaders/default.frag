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
uniform sampler2D tex;
void main()
{
	vec3 vs_v_color = vec3(1.0f);
	vec3 direction = light_pos - vert_v.xyz;
	float len = length(direction);
	direction = direction / len;
	vec3 half_vector = normalize(direction + normalize(-eye_direction));
	float diffuse_ = max(0.0, dot(normalize(vert_vn), direction));
	float specular_ = max(0.0, dot(normalize(vert_vn), half_vector));

	if (specular_ != 0.0)
	{
		specular_ = pow(specular_, shininess*4);
	}

	vec3 scattered_light = ambient + light_color * diffuse_;
	vec3 reflected_light = light_color * specular_;

	vec3 rgb = (-len / 100.0f + 1) * min(vs_v_color * scattered_light + reflected_light, vec3(1.0f));
	color = vec4(mix(max(rgb, vec3(0.0f)), texture(tex, vert_vt).rgb, 0.15f), 1.0f);
/*
	vec3 L = normalize(gl_LigntSource[0].position.xyz - vert_v);
	vec3 E = normalize(-vert_v.xyz);
	vec3 R = normalize(-reflect(L, vert_vn));
	vec4 Iamb = gl_FrontLightProduct[0].ambient;
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N, L), 0.0);
	vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R, E), 0.0), 0.3 * gl_FrontMaterial.shininess);
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
*/
}
