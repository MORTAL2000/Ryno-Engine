E(Includes/global)
E(Includes/point_include)


uniform samplerCubeShadow shadow_cube;
uniform int shadows_enabled;
uniform float shadow_strength;
uniform int index;


layout(std430, binding = 1) buffer point_ssbo
{
	PointLight lights[];
};


out vec3 fracolor;


//This function generate a depth value from the direction vector, so that it can be compared 
//with the depth value in the shadow cube

float vector_to_depth(vec3 light_vec, float n, float f)
{
	vec3 AbsVec = abs(light_vec);
	float LocalZcomp = max(AbsVec.x, max(AbsVec.y, AbsVec.z));
	
	float NormZComp = (f + n) / (f - n) - (2 * f*n) / (f - n) / LocalZcomp;
	return (NormZComp + 1.0) * 0.5;
}

void main(){

	PointLight point_light = lights[index];
	//Get uvs of the current fragment
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);
	
	//Rebuild position from depth
	float depth = texture(depth_tex, uv_coords).r *2.0-1.0;
	vec4 position_screen_space = vec4(uv_coords * 2.0 - 1.0, depth, 1);
	vec4 position_view_space_not_normalized = iP * position_screen_space;
	vec3 position_view_space = position_view_space_not_normalized.xyz / position_view_space_not_normalized.w;
	vec4 position_world_space_not_normalized = iVP * position_screen_space;
	vec3 position_world_space = position_world_space_not_normalized.xyz / position_world_space_not_normalized.w;
	vec4 light_view_space = V * point_light.position;
	
	//Color directly from g buffer
	vec4 sample_diff = texture(diffuse_tex, uv_coords);
	vec3 mat_diff = sample_diff.rgb;
	float flatness = sample_diff.w;
	vec4 sample_spec = texture(specular_tex, uv_coords);
	vec3 mat_spec = sample_spec.rgb;
	float mat_spec_pow = sample_spec.w;
	
	//Normal z-axis built back from the other two
	vec2 n = texture(normal_tex, uv_coords).xy;
	vec3 normal_view_space = vec3(n.x, n.y, sqrt(abs(1 - dot(n.xy, n.xy))));


	//Important vectors
	vec3 light_dir_world_space_not_normalized = point_light.position.xyz - position_world_space;
	vec4 light_dir_world_space = vec4(normalize(light_dir_world_space_not_normalized),0);
	vec4 light_dir_view_space = point_light.light_V_matrix * light_dir_world_space;
	vec4 view_dir_view_space = vec4(normalize(-position_view_space),0);
	vec4 half_dir_view_space = vec4(normalize(light_dir_view_space.xyz + view_dir_view_space.xyz),0);

	//Calculate attenuation
	float distance = length(light_dir_world_space_not_normalized);
	float attenuation = max(point_light.attenuation * distance * distance,1.0f);

	//Calculate base colors
	vec3 diff_color = split3(point_light.diffuse) * point_light.diffuse_intensity;
	vec3 spec_color = split3(point_light.specular) * mat_spec_pow;

	
	//final colors for diffuse and specular
	vec3 diffuse_final = max(0, dot(normal_view_space, light_dir_view_space.xyz)) * diff_color;
	vec3 specular_final = spec_color * pow(max(dot(half_dir_view_space.xyz, normal_view_space), 0.000001),  point_light.specular_intensity);
	
	//**SHADOWS**//
	float shadow = 1.0;
	if (shadows_enabled > 0.5f){
		
		//This sampling with a vec4 automatically compares the sampled value with the forth parameter (i think).
		//So the result is the shadow

		float current_depth = vector_to_depth(-light_dir_world_space_not_normalized, 1.0, point_light.max_fov);
		float bias = 0.0005;
		shadow =  texture(shadow_cube, vec4(-light_dir_world_space_not_normalized, current_depth - bias));

		//opacity
		shadow = min(1,(1-shadow_strength) + shadow);
		
	}

		

    //fragment color
	fracolor =  flatness * mat_diff + shadow *  (1.0 - flatness) * (specular_final * mat_spec + diffuse_final * mat_diff) / attenuation;
}


