#version 430 core

uniform mat4 in_View;
uniform mat4 in_Projection;

uniform vec4 in_StartColor;
uniform vec4 in_EndColor;
uniform float in_TotalLife;

layout(location = 0) in vec3 in_Position;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 4) in vec4 in_Particle_Position;
layout(location = 5) in vec4 in_Particle_Velocity;

out vec2 ex_TexCoord;
out vec4 ex_Particle_Color;

void main()
{
	vec3 CameraRight_worldspace = vec3(in_View[0][0], in_View[1][0], in_View[2][0]);
	vec3 CameraUp_worldspace = vec3(in_View[0][1], in_View[1][1], in_View[2][1]);

	vec3 newPosition = 
	vec3 (in_Particle_Position.x, in_Particle_Position.y, in_Particle_Position.z)
	+ (CameraRight_worldspace * in_Position.x * in_Particle_Position.w)
	+ (CameraUp_worldspace * in_Position.y *  in_Particle_Position.w);

	gl_Position = in_Projection * in_View * vec4(newPosition, 1.0);

	float x = 1.0 - (in_Particle_Velocity.w / in_TotalLife);
	ex_Particle_Color.x = in_StartColor.x + ((in_EndColor.x - in_StartColor.x) * x);
	ex_Particle_Color.y = in_StartColor.y + ((in_EndColor.y - in_StartColor.y) * x);
	ex_Particle_Color.z = in_StartColor.z + ((in_EndColor.z - in_StartColor.z) * x);
	ex_Particle_Color.w = in_StartColor.w + ((in_EndColor.w - in_StartColor.w) * x);

	ex_TexCoord = in_TexCoord;
}