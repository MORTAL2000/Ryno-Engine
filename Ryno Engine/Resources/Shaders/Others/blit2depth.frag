#version 430

uniform sampler2D source_buffer;
layout(std140) uniform glob_ubo {
	mat4 V;
	mat4 iV;
	mat4 P;
	mat4 iP;
	mat4 VP;
	mat4 iVP;
	vec4 cameraPos;
	float time;
	int screen_width;
	int screen_height;
};


void main(){
	vec2 uv_coords = gl_FragCoord.xy / vec2(screen_width, screen_height);

	float d = texture(source_buffer, uv_coords).x;
	gl_FragDepth = 1-d;
	
}